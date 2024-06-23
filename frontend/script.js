'use strict';

import p5 from 'p5';
console.log(p5);
import ml5 from 'ml5';
console.log(ml5);
import * as Tweakpane from 'tweakpane';
console.log;

// ###############################################
// #################### TYPES ####################
// ###############################################

/**
 * @typedef {T extends (infer U)[]
 *               ? U
 *               : T
 *          } Unpacked<T>
 * @template T
 */

/**
 * @typedef {T extends Record<string, any>
 *               ? keyof T
 *               : never
 *          } ExtractKeys<T>
 * @template T
 */

/**
 * @typedef {T extends Record<string, any>
 *               ? keyof T | ExtractKeysDeep<T[keyof T]>
 *               : never
 *          } ExtractKeysDeep<T>
 * @template T
 */

/**
 * @typedef {Object} DeviceSettings
 * 
 * @property {bigint} magic
 * 
 * @property {Object} wifi
 * @property {number} wifi.mode
 * @property {number} wifi.timeout
 * @property {string} wifi.hostname
 * @property {number} wifi.security
 * 
 * @property {Object} wifi.ap
 * @property {string} wifi.ap.ssid
 * @property {string} wifi.ap.pass
 * @property {string} wifi.ap.local_ip
 * @property {string} wifi.ap.gateway
 * @property {string} wifi.ap.subnet
 * 
 * @property {Object} wifi.sta
 * @property {string} wifi.sta.ssid
 * @property {string} wifi.sta.pass
 * @property {boolean} wifi.sta.dhcp
 * @property {string} wifi.sta.local_ip
 * @property {string} wifi.sta.gateway
 * @property {string} wifi.sta.subnet
 * @property {string} wifi.sta.dns1
 * @property {string} wifi.sta.dns2
 * 
 * @property {Object} ota
 * @property {string} ota.path
 * @property {string} ota.username
 * @property {string} ota.password
 * 
 * @property {Object} camera
 * @property {number} camera.xclk_freq_hz
 * @property {number} camera.ledc_timer
 * @property {number} camera.ledc_channel
 * @property {number} camera.pixel_format
 * @property {number} camera.frame_size
 * @property {number} camera.jpeg_quality
 * @property {number} camera.fb_count
 * @property {number} camera.fb_location
 * @property {number} camera.grab_mode
 * 
 * @property {Object} $types
 * 
 * @property {Object} $types.wifi
 * 
 * @property {Object} $types.wifi.mode
 * @property {Array<
 *                "NULL" | "STA" | "AP" | "APSTA" | "NAN" | "MAX"
 *            >} $types.wifi.mode.$enum
 * 
 * @property {Object} $types.wifi.security
 * @property {Array<
 *                "OPEN" | "WEP" | "WPA_PSK" | "WPA2_PSK" | "WPA_WPA2_PSK" | "WPA2_ENTERPRISE" | "WPA3_PSK" | "WPA2_WPA3_PSK" | "WAPI_PSK" | "OWE" | "WPA3_ENT_192" | "MAX"
 *            >} $types.wifi.security.$enum
 * 
 * @property {Object} $types.camera
 * @property {Array<
 *                "RGB565" | "YUV422" | "YUV420" | "GRAYSCALE" | "JPEG" | "RGB888" | "RAW" | "RGB444" | "RGB555"
 *            >} $types.camera.pixel_format
 * @property {Array<
 *                "96X96" | "QQVGA" | "QCIF" | "HQVGA" | "240X240" | "QVGA" | "CIF" | "HVGA" | "VGA" | "SVGA" | "XGA" | "HD" | "SXGA" | "UXGA" | "FHD" | "P_HD" | "P_3MP" | "QXGA" | "QHD" | "WQXGA" | "P_FHD" | "QSXGA"
 *            >} $types.camera.frame_size
 * @property {Array<
 *                "IN_PSRAM" | "IN_DRAM"
 *            >} $types.camera.fb_location
 * @property {Array<
 *                "WHEN_EMPTY" | "LATEST"
 *            >} $types.camera.grab_mode
 */

/**
 * @typedef {Object} SensorSettings
 * @property {number} pixformat
 * @property {number} framesize
 * @property {number} brightness
 * @property {number} contrast
 * @property {number} saturation
 * @property {number} sharpness
 * @property {number} denoise
 * @property {number} gainceiling
 * @property {number} quality
 * @property {number} colorbar
 * @property {number} whitebal
 * @property {number} gain_ctrl
 * @property {number} exposure_ctrl
 * @property {number} hmirror
 * @property {number} vflip
 * @property {number} aec2
 * @property {number} awb_gain
 * @property {number} agc_gain
 * @property {number} aec_value
 * @property {number} special_effect
 * @property {number} wb_mode
 * @property {number} ae_level
 * @property {number} dcw
 * @property {number} bpc
 * @property {number} wpc
 * @property {number} raw_gma
 * @property {number} lenc
 */

//#region OpenAPI JSON

/**
 * @typedef {Object} OpenApiInfo
 * @property {string} title
 * @property {string} description
 * @property {string} [termsOfService]
 * @property {string} version
 */

/**
 * @typedef {Object} OpenApiServer
 * @property {string} url
 * @property {Object.<string, { enum: number[], default: string }>} [variables]
 */

/**
 * @typedef {Object} OpenApiTag
 * @property {string} name
 * @property {string} description
 */

/**
 * @typedef {Object} OpenApiResponse
 * @property {string} description
 * @property {Object.<string, { schema: { type: string, format?: string } } | { schema: { $ref: string } }>} [content]
 */

/**
 * @typedef {Object} OpenApiRequestBody
 * @property {string} [description]
 * @property {Object.<string, { schema: { $ref: string } }>} content
 * @property {boolean} required
 */

/**
 * @typedef {Object} OpenApiOperation
 * @property {string[]} tags
 * @property {string} summary
 * @property {string} description
 * @property {string} operationId
 * @property {Object.<string, OpenApiResponse>} responses
 * @property {OpenApiRequestBody} [requestBody]
 */

/**
 * @typedef {Object} OpenApiProto
 * @property {OpenApiOperation} [get]
 * @property {OpenApiOperation} [put]
 * @property {OpenApiOperation} [post]
 * @property {OpenApiOperation} [delete]
 * @property {OpenApiOperation} [options]
 * @property {OpenApiOperation} [head]
 * @property {OpenApiOperation} [patch]
 * @property {OpenApiOperation} [trace]
 */

/**
 * @typedef {{ [key: string]: OpenApiProto }} OpenApiPath
 */

/**
* @typedef {Object} OpenApiProperty
* @property {string} type
* @property {string} [format]
* @property {number} [minimum]
* @property {number} [maximum]
*/

/**
* @typedef {Object} OpenApiSchema
* @property {string} type
* @property {Object.<string, OpenApiProperty>} properties
*/

/**
* @typedef {Object} OpenApiComponent
* @property {Object.<string, OpenApiSchema>} schemas
*/

/**
* @typedef {Object} OpenApiJson
* @property {string} openapi
* @property {OpenApiInfo} info
* @property {OpenApiServer[]} servers
* @property {OpenApiTag[]} tags
* @property {OpenApiPath} paths
* @property {OpenApiComponent} components
*/

//#endregion OpenAPI JSON

/**
 * @typedef {{ w: number, h: number; }} Size
 */

/**
 * @typedef {Object} Constants
 * 
 * @property {Object} endpoints
 * @property {URL} endpoints.stream
 * @property {URL} endpoints.settings
 * @property {URL} endpoints.sensor
 * @property {URL} endpoints.openapi
 * 
 * @property {string} font
 */

/**
 * @typedef {Object} Variables
 *
 * @property {ml5.ObjectDetectorModel} model
 * 
 * @property {p5.Element?} image
 * @property {ObjectDetector?} od
 * 
 * @property {Object} size
 * @property {{ w: number, h: number; }} size.original
 * @property {{ w: number, h: number; }} size.scaled
 * 
 * @property {Object} [settings]
 * @property {DeviceSettings} settings.device
 * @property {SensorSettings} settings.sensor
 */

/**
 * @typedef {T extends keyof DeviceSettings ? T : null} DeviceSettingsBindingParentKey<T>
 * @template T
 */

/**
 * @typedef {T extends keyof DeviceSettings ? K : T extends null ? ExtractKeysDeep<DeviceSettings> : never} DeviceSettingsBindingSubKey<T, K>
 * @template {keyof DeviceSettings | null} T
 * @template K
 */

/**
 * @typedef {Object} Exports
 * @property {p5} [app]
 * @property {UI} [ui]
 * @property {(p: p5) => void} sketch
 * @property {() => void} setup
 * 
 * @property {Object} modules
 * @property {typeof p5} modules.p5
 * @property {typeof ml5} modules.ml5
 * @property {typeof Tweakpane} modules.Tweakpane
 * 
 * @property {Object} test
 * @property {typeof ObjectDetector} test.od
 * @property {typeof UI} test.ui
 * @property {typeof fetchSettings} test.fs
 * @property {typeof sendSettings} test.ss
 */

// ###############################################
// ################### HELPERS ###################
// ###############################################

/**
 * @param {URL} endpoint 
 * @returns {Promise<DeviceSettings | SensorSettings | OpenApiJson | null>}
 */
async function fetchSettings(endpoint) {
    try {
        console.log(`Fetching settings from ${endpoint}`);
        const response = await fetch(endpoint, {
            cache: "no-cache",
            method: "GET"
        });
        if (!response.ok) {
            throw new Error('(' + response.status + ') ' + response.statusText);
        }

        /** @type {DeviceSettings | SensorSettings | null} */
        let json = null;
        const text = await response.text();
        const magic = text.match(/"magic"\s*:\s*(?<magic>\d+)/);
        if (magic) {
            const magicValue = magic.groups?.magic;
            if (magicValue) {
                const magicInt = BigInt(magicValue);
                json = /** @type {DeviceSettings | null} */ (JSON.parse(text));
                if (json && json.magic) {
                    json.magic = magicInt;
                }
            }
        }

        if (!json)
            json = JSON.parse(text);

        return json;
    } catch (reason) {
        if (reason instanceof Error) {
            console.error('Failed to fetch settings:\n', reason.message);
        } else {
            console.error(reason);
        }
        throw reason;
    }
}

/**
 * @param {URL} endpoint 
 * @param {DeviceSettings | SensorSettings | Object} config
 */
async function sendSettings(endpoint, config) {
    console.log("Sending settings:", config); // Debugging log
    try {
        const response = await fetch(endpoint, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(config, (_, v) => typeof v === 'bigint' ? v.toString() : v)
        });
        if (!response.ok) {
            throw new Error('Failed to send configuration data.');
        }
        console.log('Sensor configuration data sent.');
    } catch (reason) {
        if (reason instanceof Error) {
            console.error('Failed to send settings:\n', reason.message);
        } else {
            console.error(reason);
        }
        throw reason;
    }
}

// ###############################################
// ################### CLASSES ###################
// ###############################################

class ObjectDetector {
    /** @type {ml5.ObjectDetectorModel?} */
    static model = null;
    /** @type {ml5.ObjectDetector?} */
    static detector = null;

    /** @type {p5.Element?} */
    image = null;
    /** @type {Array<ml5.ObjectDetectorPrediction>} */
    detections = [];

    /**
     * @param {ml5.ObjectDetectorModel} model
     * @param {p5.Element} image
     */
    constructor(model, image) {
        this.image = image;
        if (ObjectDetector.model !== model) {
            ObjectDetector.model = model;
            ObjectDetector.detector = null;
        }
        this.videoReady();
    }

    /**
     * @param {ml5.ObjectDetectorModel} model
     */
    static preload(model) {
        ml5.objectDetector(model)
            .then((detector) => {
                ObjectDetector.model = model;
                ObjectDetector.detector = detector;
            })
            .catch((error) => {
                console.error(error);
            });
    }

    destroy() {
        this.detections = [];
        this.image = null;
    }

    /**
     * @param {any | undefined} error
     * @param {Array<ml5.ObjectDetectorPrediction>} results
     */
    gotDetections(error, results) {
        if (error) {
            console.error(error);
        }
        this.detections = results;
        if (ObjectDetector.detector && this.image)
            ObjectDetector.detector.detect(this.image, this.gotDetections.bind(this));
    }

    modelReady() {
        if (ObjectDetector.detector && this.image)
            ObjectDetector.detector.detect(this.image, this.gotDetections.bind(this));
    }

    videoReady() {
        if (ObjectDetector.model) {
            if (ObjectDetector.detector)
                this.modelReady();
            else
                ObjectDetector.detector = ml5.objectDetector(ObjectDetector.model, this.modelReady.bind(this));
        }
    }
}

class UI {
    /** @type {Tweakpane.Pane} */
    pane;

    /**
     * @param {DeviceSettings} deviceConfig
     * @param {SensorSettings} sensorConfig
     * @param {OpenApiJson} openapiJson
     */
    constructor(deviceConfig, sensorConfig, openapiJson) {
        const tweakpaneContainer = document.getElementById('tweakpane-container');
        if (!tweakpaneContainer) {
            throw new Error("Element '#tweakpane-container' not found.");
        }

        this.pane = new Tweakpane.Pane({
            container: tweakpaneContainer,
            title: "ESP32-CAM-ML",
            expanded: true
        });

        window.matchMedia('(max-width: 768px)').addEventListener('change', (ev) => {
            if (ev.matches)
                this.pane.controller.foldable.set('expanded', false);
            else
                this.pane.controller.foldable.set('expanded', true);
        });

        this.pane.addButton({
            title: "Start Object Detection"
        })
            .on("click", () => {
                if (!vars.od?.image && vars.image) {
                    vars.od = new ObjectDetector(vars.model, vars.image);
                }
            });

        this.pane.addButton({
            title: "Stop Object Detection"
        })
            .on("click", () => {
                if (vars.od) {
                    vars.od.destroy();
                }
            });

        const mainFolder = this.pane.addFolder({ title: 'Main Settings', expanded: true });
        const device_config_keys = /** @type {Array<keyof typeof deviceConfig & string>} */ (Object.keys(deviceConfig));
        device_config_keys.forEach((key) => {
            if (key === "$types" || key === "magic") {
                return;
            }

            const folder = mainFolder.addFolder({ title: key.charAt(0).toUpperCase() + key.slice(1), expanded: true });
            switch (key) {
                case "wifi": {
                    const setting = deviceConfig[key];
                    const wifi_keys = /** @type {Array<ExtractKeys<DeviceSettings["wifi"]> & string>} */ (Object.keys(setting));
                    wifi_keys.forEach((subkey) => {
                        if (typeof setting[subkey] === 'object' && setting[subkey] !== null) {
                            const subfolder = folder.addFolder({ title: subkey.charAt(0).toUpperCase() + subkey.slice(1), expanded: true });
                            const temp = setting[subkey];
                            const wifi_subkeys = /** @type {Array<ExtractKeys<typeof temp> & string>} */ (Object.keys(setting[subkey]));
                            wifi_subkeys.forEach(subkey => {
                                this.addDeviceBinding(subfolder, temp, deviceConfig.$types, null, subkey);
                            });
                        } else {
                            this.addDeviceBinding(folder, setting, deviceConfig.$types, key, subkey);
                        }
                    });
                } break;
                case "ota": {
                    const setting = deviceConfig[key];
                    const ota_keys = /** @type {Array<keyof typeof deviceConfig.ota & string>} */ (Object.keys(deviceConfig.ota));
                    ota_keys.forEach(subkey => {
                        this.addDeviceBinding(folder, setting, deviceConfig.$types, key, subkey);
                    });
                } break;
                case "camera": {
                    const setting = deviceConfig[key];
                    const camera_keys = /** @type {Array<keyof typeof deviceConfig.camera & string>} */ (Object.keys(deviceConfig.camera));
                    camera_keys.forEach(subkey => {
                        if (subkey === "xclk_freq_hz" || subkey === "ledc_timer" || subkey === "ledc_channel") {
                            return;
                        }
                        this.addDeviceBinding(folder, setting, deviceConfig.$types, key, subkey);
                    });
                } break;
                default: {
                    console.warn(`Unknown key: ${key}`);
                    const unknownfolder = mainFolder.addFolder({ title: "Unknown", expanded: true });
                    this.addDeviceBinding(unknownfolder, deviceConfig, deviceConfig.$types, null, key);
                }
            }
        });

        const sensorFolder = this.pane.addFolder({ title: 'Sensor Settings', expanded: true });
        this.createSensorBindings(sensorFolder, sensorConfig, openapiJson);
    }

    destroy() {
        this.pane.dispose();
    }

    /**
     * Creates bindings for sensor settings based on OpenAPI JSON schema
     * @param {Tweakpane.FolderApi} folder
     * @param {SensorSettings} sensorConfig
     * @param {OpenApiJson} openapiJson
     */
    createSensorBindings(folder, sensorConfig, openapiJson) {
        const sensorProps = /** @type {OpenApiSchema["properties"]} */ (openapiJson.components.schemas["Sensor"].properties);
        const sensorKeys = /** @type {Array<keyof SensorSettings & string>} */ (Object.keys(sensorProps));
        sensorKeys.forEach(key => {
            const property = sensorProps[key];
            if (property.minimum !== undefined && property.maximum !== undefined) {
                this.addSensorRangeBinding(folder, sensorConfig, key, { min: property.minimum, max: property.maximum });
            } else {
                this.addSensorTextBinding(folder, sensorConfig, key);
            }
        });
    }

    /**
     * @template {keyof DeviceSettings | null} T
     * @template {string | ExtractKeysDeep<DeviceSettings["$types"]>} K
     * @param {Tweakpane.FolderApi} folder
     * @param {Omit<DeviceSettings[keyof DeviceSettings], "magic" | "$types">} setting
     * @param {DeviceSettings["$types"]} types
     * @param {DeviceSettingsBindingParentKey<T>} parentKey 
     * @param {DeviceSettingsBindingSubKey<T, K> & string} subkey 
     */
    addDeviceBinding(folder, setting, types, parentKey, subkey) {
        let typePath;
        if (parentKey) {
            const type_parent_key = /** @type {keyof DeviceSettings} */ (parentKey);
            switch (type_parent_key) {
                case "wifi": {
                    const type_sub_key = /** @type {ExtractKeys<DeviceSettings["$types"]["wifi"]>} */ (subkey);
                    typePath = types?.[type_parent_key]?.[type_sub_key]?.$enum;
                } break;
                case "camera": {
                    const type_sub_key = /** @type {ExtractKeys<DeviceSettings["$types"]["camera"]>} */ (subkey);
                    typePath = types?.[type_parent_key]?.[type_sub_key];
                } break;
                case "ota":
                    break;
                default: {
                    console.warn(`Unknown parent key: ${parentKey}`);
                    //throw new Error(`Unknown parent key: ${parentKey}`);
                }
            }
        }

        if (typePath) {
            const options = typePath.reduce((acc, option, index) => {
                Object.assign(acc, { [option]: index });
                return acc;
            }, {});
            folder.addBinding(setting, /** @type {never} */(subkey), {
                label: subkey.charAt(0).toUpperCase() + subkey.slice(1),
                options: options
            }).on('change', (ev) => {
                Object.assign(setting, { [subkey]: ev.value });
                console.log(`Selected ${subkey}:`, Object.keys(options)[ev.value], "->", ev.value);
                if (vars.settings) {
                    sendSettings(consts.endpoints.settings, vars.settings?.device);
                }
            });
        } else {
            folder.addBinding(setting, /** @type {never} */(subkey), {
                label: subkey.charAt(0).toUpperCase() + subkey.slice(1)
            }).on('change', () => {
                if (vars.settings)
                    sendSettings(consts.endpoints.settings, vars.settings?.device);
            });
        }
    }

    /**
     * @param {Tweakpane.FolderApi} folder
     * @param {SensorSettings} setting
     * @param {keyof SensorSettings & string} key
     */
    addSensorTextBinding(folder, setting, key) {
        folder.addBinding(setting, key, {
            label: key.charAt(0).toUpperCase() + key.slice(1)
        }).on('change', () => {
            if (vars.settings)
                sendSettings(consts.endpoints.settings, vars.settings?.sensor);
        });
    }

    /**
     * @param {Tweakpane.FolderApi} folder
     * @param {SensorSettings} setting
     * @param {keyof SensorSettings & string} key
     * @param {{ min: number, max: number }} range
     */
    addSensorRangeBinding(folder, setting, key, range) {
        folder.addBinding(setting, key, {
            label: key.charAt(0).toUpperCase() + key.slice(1),
            min: range.min,
            max: range.max
        }).on('change', () => {
            if (vars.settings)
                sendSettings(consts.endpoints.settings, vars.settings?.sensor);
        });
    }
}

// ###############################################
// ################## CONSTANTS ##################
// ###############################################

const appBaseUrl = (() => {
    let schema = document.location.protocol;
    if (schema !== "http:" && schema !== "https:") {
        throw new Error("Invalid protocol.");
    }
    let hostname = (document.location.hostname !== "127.0.0.1" && document.location.hostname !== "localhost")
        ? document.location.hostname
        : "192.168.69.1";
    return new URL(`${schema}//${hostname}`);
})();

const streamBaseUrl = (() => {
    const stream = new URL(appBaseUrl);
    stream.port = "81";
    return stream;
})();

/**
 * @type {Constants}
 */
const consts = {
    endpoints: {
        stream: new URL("/stream", streamBaseUrl),
        settings: new URL("/settings", appBaseUrl),
        sensor: new URL("/sensor", appBaseUrl),
        openapi: new URL("/api/openapi.json", appBaseUrl),
    },
    font: "assets/FiraMonoNerdFontMono-Bold.otf",
};

// ###############################################
// ################## VARIABLES ##################
// ###############################################

/**
 * @type {Variables}
 */
const vars = {
    model: "CocoSsd",
    image: null,
    od: null,
    size: {
        original: { w: 800, h: 600 },
        scaled: { w: 800, h: 600 },
    },
};

// ###############################################
// ################## App Logic ##################
// ###############################################

/**
 * @param {p5} p
 */
const sketch = (p) => {
    /** @type {p5.Renderer?} */
    let renderer = null;
    /** @type {p5.Font?} */
    let font = null;

    /**
     * @param {Size} original
     * @param {Size} box
     * @returns {Size} Scaled size
     */
    function resizeVideo(original, box) {
        // Preserve aspect ratio and fit into box
        const box_ratio = box.w / box.h;
        const original_ratio = original.w / original.h;
        if (original_ratio > box_ratio) {
            return {
                w: box.w,
                h: box.w / original_ratio
            };
        } else {
            return {
                w: box.h * original_ratio,
                h: box.h
            };
        }
    }

    p.windowResized = () => {
        p.resizeCanvas(renderer?.elt.offsetWidth, renderer?.elt.offsetHeight);
        if (vars.image) {
            const img = /** @type {HTMLImageElement} */ (vars.image.elt);
            vars.size.original = { w: img.naturalWidth, h: img.naturalHeight };
            vars.image.size(vars.size.original.w, vars.size.original.h);
        }
        vars.size.scaled = resizeVideo(vars.size.original, {
            w: renderer?.elt.offsetWidth,
            h: renderer?.elt.offsetHeight
        });
    };

    p.setup = () => {
        const el = document.getElementById('p5-container');
        if (!el) {
            throw new Error("Element '#p5-container' not found.");
        }
        renderer = p.createCanvas(el.offsetWidth, el.offsetHeight, p.WEBGL, el);
        vars.image = p.createImg(consts.endpoints.stream.toString(), "Stream", "", /** @param {p5.Element} el */(el) => {
            const img = /** @type {HTMLImageElement} */ (el.elt);
            vars.size.original = { w: img.naturalWidth, h: img.naturalHeight };
            el.size(vars.size.original.w, vars.size.original.h);
            el.hide();
            el.center();
            p.windowResized();
            setInterval(() => {
                p.windowResized();
            }, 250);
        });
        font = p.loadFont(consts.font, () => {
            if (font) p.textFont(font);
        });
    };

    p.draw = () => {
        if (vars.image) {
            p.background(0);

            const origin = {
                x: -vars.size.scaled.w / 2,
                y: -vars.size.scaled.h / 2
            };

            p.textureMode(p.NORMAL);
            p.texture(/** @type {p5.MediaElement} */(vars.image));
            p.rect(origin.x, origin.y, vars.size.scaled.w, vars.size.scaled.h);

            if (vars.od?.image && vars.od.detections.length > 0) {
                const xRatio = vars.size.scaled.w / vars.size.original.w;
                const yRatio = vars.size.scaled.h / vars.size.original.h;

                for (let i = 0; i < vars.od.detections.length; i++) {
                    const object = vars.od.detections[i];

                    const pos = {
                        x: origin.x + object.x * xRatio,
                        y: origin.y + object.y * yRatio,
                        w: object.width * xRatio,
                        h: object.height * yRatio
                    };

                    p.push();

                    p.stroke(255, 0, 255, 255 / 2);
                    p.strokeWeight(2);
                    p.noFill();
                    p.rect(
                        pos.x,
                        pos.y,
                        pos.w,
                        pos.h
                    );
                    p.noStroke();
                    p.fill(255);

                    p.textSize(20);
                    p.text(
                        `${object.label} (${(object.confidence * 100).toFixed(1)} %)`,
                        pos.x + 10,
                        pos.y + 20
                    );

                    p.pop();
                }
            }
        }
    };
};

// ##############################################
// #################### INIT ####################
// ##############################################

/**
 * @type {Exports}
 */
const exports = {
    sketch: sketch,
    setup: async () => {
        try {
            vars.settings = {
                device: /** @type {DeviceSettings} */ (await fetchSettings(consts.endpoints.settings)),
                sensor: /** @type {SensorSettings} */ (await fetchSettings(consts.endpoints.sensor)),
            };
            let openapiJson = /** @type {OpenApiJson} */ (await fetchSettings(consts.endpoints.openapi));

            console.log('Fetched settings:', vars.settings.device);
            console.log('Fetched sensor settings:', vars.settings.sensor);

            ObjectDetector.preload(vars.model);
            exports.app = new p5(sketch);
            exports.ui = new UI(vars.settings.device, vars.settings.sensor, openapiJson);
        } catch (error) {
            console.error('Error setting up ESP32-CAM-ML:', error);
        }
    },
    modules: {
        p5,
        ml5,
        Tweakpane,
    },
    test: {
        od: ObjectDetector,
        ui: UI,
        fs: fetchSettings,
        ss: sendSettings,
    }
};

globalThis.exports = {
    esp32: exports
};
