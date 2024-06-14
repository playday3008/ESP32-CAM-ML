'use strict';

import p5 from 'p5';
console.log(p5);
import ml5 from 'ml5';
console.log(ml5);
import * as Tweakpane from 'tweakpane';
console.log(Tweakpane);

/**
 * @type {ml5.ObjectDetectorModel}
 */
const model = "CocoSsd";
const schema = document.location.protocol;
const host = document.location.hostname;
const port = 81;
const path = "/stream";
const source = new URL(`${schema}//${host}:${port}${path}`);

/**
 * p5.js sketch object.
 * @param {p5} p 
 */
const sketch = p => {
  /**
   * @type {p5.Element}
   */
  let frame;
  /**
   * @type {ml5.ObjectDetector}
   */
  let detector;
  /**
   * @type {Array<ml5.ObjectDetectorPrediction>}
   */
  let detections = [];

  /**
   * 
   * @param {any | undefined} error Error object.
   * @param {any} results Result object.
   */
  function gotDetections(error, results) {
    if (error) {
      console.error(error);
    }
    detections = /** @type {Array<ml5.ObjectDetectorPrediction>} */(results);
    detector.detect(frame, gotDetections);
  }

  function modelReady() {
    detector.detect(frame, gotDetections);
  }

  function videoReady() {
    detector = ml5.objectDetector(model, modelReady);
  }

  p.setup = function () {
    p.createCanvas(640, 480);
    frame = p.createImg(source.toString(), "Stream", "", videoReady);
    frame.size(640, 480);
    frame.hide();
  };

  p.draw = function () {
    p.image(frame, 0, 0);

    for (let i = 0; i < detections.length; i++) {
      const object = detections[i];
      p.stroke(0, 255, 0);
      p.strokeWeight(1);
      p.noFill();
      p.rect(object.x, object.y, object.width, object.height);
      p.noStroke();
      p.fill(255);
      p.textSize(24);
      p.text(object.label, object.x + 10, object.y + 24);
    }
  };
};

/**
 * @returns {Tweakpane.Pane}
 */
const ui = () => {
  const pane = new Tweakpane.Pane({
    title: "ESP32-CAM-ML",
    expanded: true
  });
  return pane;
}

globalThis.esp32 = new Object({
  /**
   * @type {p5?}
   */
  app: null,
  /**
   * @type {Tweakpane.Pane?}
   */
  ui: null,
  setup: () => {
    return new Object({
      app: new p5(sketch),
      ui: ui()
    })
  },
  "p5": p5,
  "ml5": ml5,
  "Tweakpane": Tweakpane,
  "sketch": sketch
});
