import p5 from "p5";

export as namespace ml5;

type YoloOptions = {
    classProbThreshold: number;
    filterBoxesThreshold: number;
    IOUThreshold: number;
};

type ObjectDetectorModel = "YOLO" | "CocoSsd" | URL;

type ObjectDetectorPrediction = {
    /*
     * Top left corner x coordinate of prediction box
     */
    x: number;

    /*
     * Top left corner y coordinate of prediction box
     */
    y: number;

    /*
     * Width of prediction box
     */
    width: number;

    /*
     * Height of prediction box
     */
    height: number;

    /*
     * Label given to the object
     */
    label: string;

    /*
     * Confidence score of the prediction (0 to 1)
     */
    confidence: number;

    /*
     * Normalized coordinates of the prediction box
     */
    normalized: {
        /* 
         * Normalized x coordinate of the prediction box
         */
        x: number;

        /* 
         * Normalized y coordinate of the prediction box
         */
        y: number;

        /* 
         * Normalized width of the prediction box
         */
        width: number;

        /* 
         * Normalized height of the prediction box
         */
        height: number;
    };
};

class ObjectDetector {
    constructor(
        modelNameOrUrl: ObjectDetectorModel,
        options?: YoloOptions | {},
        callback?: (error: any, result: Array<ObjectDetectorPrediction>) => void
    );
    detect(
        input: p5.Element | p5.MediaElement | HTMLCanvasElement | HTMLImageElement | HTMLVideoElement | ImageData,
        callback: (error: any | undefined, result: Array<ObjectDetectorPrediction>) => void
    ): void;
}

export function objectDetector(
    modelNameOrUrl: ObjectDetectorModel,
    callback: (error: any | undefined, result: any) => void
): ObjectDetector;

export function objectDetector(
    modelNameOrUrl: ObjectDetectorModel,
    options: YoloOptions | {},
    callback: (error: any | undefined, result: any) => void
): ObjectDetector;

export function objectDetector(
    modelNameOrUrl: ObjectDetectorModel,
    options?: YoloOptions | {},
): Promise<ObjectDetector>;
