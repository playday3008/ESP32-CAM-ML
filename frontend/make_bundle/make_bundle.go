package main

// Usage:
// cd make_bundle
// go run make_bundle.go -i ../index.html -o ../bundle.html.gz -c

import (
	"compress/gzip"
	"flag"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"

	"net/url"

	"golang.org/x/net/html"

	minify "github.com/tdewolff/minify/v2"
	minify_css "github.com/tdewolff/minify/v2/css"
	minify_html "github.com/tdewolff/minify/v2/html"
	minify_js "github.com/tdewolff/minify/v2/js"
	minify_json "github.com/tdewolff/minify/v2/json"
	minify_svg "github.com/tdewolff/minify/v2/svg"
	minify_xml "github.com/tdewolff/minify/v2/xml"
)

var minifier = minify.New()

func init() {
	minifier.Add("text/html", &minify_html.Minifier{
		KeepDocumentTags: true,
		KeepEndTags:      true,
		KeepQuotes:       true,
	})
	minifier.AddFunc("text/css", minify_css.Minify)
	minifier.AddFunc("text/javascript", minify_js.Minify)
	minifier.AddFunc("text/json", minify_json.Minify)
	minifier.AddFunc("image/svg+xml", minify_svg.Minify)
	minifier.AddFunc("text/xml", minify_xml.Minify)
}

func bundle(root *html.Node, input string) {
	// For each node recursively
	for node := root.FirstChild; node != nil; node = node.NextSibling {
		// If node is a script or link tag
		if node.Type == html.ElementNode && (node.Data == "script" || node.Data == "link") {
			// For each attribute
			for i, a := range node.Attr {
				// If attribute is src or href
				var path string
				switch {
				case node.Data == "link" && a.Key == "href":
					for _, attr := range node.Attr {
						if attr.Key == "rel" && attr.Val == "stylesheet" {
							goto do_css
						}
					}
					continue
				do_css:
					fallthrough
				case node.Data == "script" && a.Key == "src":
					u, err := url.Parse(a.Val)
					if err != nil {
						log.Fatal(err)
					}
					if u.IsAbs() {
						// TODO: Implement HTTP GET request to download external files
						continue
					}
					path = u.Path
				default:
					continue
				}
				// Stat file
				fi, err := os.Stat(filepath.Join(filepath.Dir(input), path))
				if err != nil {
					log.Fatal(err)
				}

				// Open file
				f, err := os.Open(filepath.Join(filepath.Dir(input), path))
				if err != nil {
					log.Fatal(err)
				}
				defer f.Close()

				// Read file
				b := make([]byte, fi.Size())
				n, err := f.Read(b)
				if err != nil {
					log.Fatal(err)
				}

				// Create text node
				text := &html.Node{
					Type: html.TextNode,
					Data: string(b[:n]),
				}

				switch {
				case node.Data == "script":
					// Remove attribute
					node.Attr = append(node.Attr[:i], node.Attr[i+1:]...)

					// Miniify JavaScript
					text.Data, err = minifier.String("text/javascript", text.Data)
					if err != nil {
						log.Fatal(err)
					}

					// Append text node
					node.AppendChild(text)
				case node.Data == "link":
					// Miniify CSS
					text.Data, err = minifier.String("text/css", text.Data)
					if err != nil {
						log.Fatal(err)
					}

					// Replace node
					node.Data = "style"
					node.Attr = nil
					node.FirstChild = nil
					node.LastChild = nil

					// Append text node
					node.AppendChild(text)
				}
			}
		} else {
			bundle(node, input)
		}
	}
}

func minifyFile(input string, output string) (err error) {
	// Open input file
	in, err := os.Open(input)
	if err != nil {
		return
	}
	defer in.Close()

	// Parse input file
	doc, err := html.Parse(in)
	if err != nil {
		return
	}

	// Create output file
	out, err := os.Create(output)
	if err != nil {
		return
	}
	defer out.Close()

	// Create gzip output writer
	gz := gzip.NewWriter(out)
	defer gz.Close()

	// Bundle JavaScript and CSS files into HTML
	bundle(doc, input)

	// Create pipe
	ior, iow := io.Pipe()

	// Write modified HTML
	go func() {
		if err := html.Render(iow, doc); err != nil {
			return
		}
		iow.Close()
	}()

	// Minify HTML
	if err = minifier.Minify("text/html", gz, ior); err != nil {
		return
	}

	return
}

func main() {
	// Parse command line flags
	input_flag_ptr := flag.String("i", "../index.html", "Input HTML file")
	output_flag_ptr := flag.String("o", "../bundle.html.gz", "Output HTML file")
	c_array_flag_ptr := flag.Bool("c", false, "Generate C style array of bytes")

	flag.Parse()

	input_flag, err := filepath.Abs(*input_flag_ptr)
	if err != nil {
		log.Fatal(err)
	}
	output_flag, err := filepath.Abs(*output_flag_ptr)
	if err != nil {
		log.Fatal(err)
	}

	if err := minifyFile(input_flag, output_flag); err != nil {
		log.Fatal(err)
	}

	// Open output file
	f, err := os.Open(output_flag)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	// Stat file
	fi, err := os.Stat(output_flag)
	if err != nil {
		log.Fatal(err)
	}

	// Print C style array of bytes
	if *c_array_flag_ptr {
		fmt.Printf("#define bundle_stub_html_gz_size %d\n", fi.Size())
		fmt.Printf("const uint8_t bundle_stub_html_gz[bundle_stub_html_gz_size] = {\n")
		for {
			b := make([]byte, 16)
			n, err := f.Read(b)
			if err == io.EOF {
				break
			}
			if err != nil {
				log.Fatal(err)
			}
			fmt.Printf("\t")
			for i := 0; i < n; i++ {
				fmt.Printf("0x%02x, ", b[i])
			}
			fmt.Printf("\n")
		}
		fmt.Printf("};\n")
	}

	log.Printf("Bundle created: %s", output_flag)
}
