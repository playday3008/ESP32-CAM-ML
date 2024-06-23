'use strict';

import QUnit from "qunit";
import p5 from 'p5';
import ml5 from 'ml5';
import * as Tweakpane from 'tweakpane';
import '../script.js';

var exports = globalThis.exports.esp32;

var ObjectDetector = exports.test.od;
var UI = exports.test.ui;
var fetchSettings = exports.test.fs;
var sendSettings = exports.test.ss;

const testImage = new URL("https://raw.githubusercontent.com/tensorflow/models/master/research/object_detection/test_images/image2.jpg");

const petstoreJsonUrl = new URL("https://petstore.swagger.io/v2/swagger.json");

const petstore = { "swagger": "2.0", "info": { "description": "This is a sample server Petstore server.  You can find out more about Swagger at [http://swagger.io](http://swagger.io) or on [irc.freenode.net, #swagger](http://swagger.io/irc/).  For this sample, you can use the api key `special-key` to test the authorization filters.", "version": "1.0.7", "title": "Swagger Petstore", "termsOfService": "http://swagger.io/terms/", "contact": { "email": "apiteam@swagger.io" }, "license": { "name": "Apache 2.0", "url": "http://www.apache.org/licenses/LICENSE-2.0.html" } }, "host": "petstore.swagger.io", "basePath": "/v2", "tags": [{ "name": "pet", "description": "Everything about your Pets", "externalDocs": { "description": "Find out more", "url": "http://swagger.io" } }, { "name": "store", "description": "Access to Petstore orders" }, { "name": "user", "description": "Operations about user", "externalDocs": { "description": "Find out more about our store", "url": "http://swagger.io" } }], "schemes": ["https", "http"], "paths": { "/pet/{petId}/uploadImage": { "post": { "tags": ["pet"], "summary": "uploads an image", "description": "", "operationId": "uploadFile", "consumes": ["multipart/form-data"], "produces": ["application/json"], "parameters": [{ "name": "petId", "in": "path", "description": "ID of pet to update", "required": true, "type": "integer", "format": "int64" }, { "name": "additionalMetadata", "in": "formData", "description": "Additional data to pass to server", "required": false, "type": "string" }, { "name": "file", "in": "formData", "description": "file to upload", "required": false, "type": "file" }], "responses": { "200": { "description": "successful operation", "schema": { "$ref": "#/definitions/ApiResponse" } } }, "security": [{ "petstore_auth": ["write:pets", "read:pets"] }] } }, "/pet": { "post": { "tags": ["pet"], "summary": "Add a new pet to the store", "description": "", "operationId": "addPet", "consumes": ["application/json", "application/xml"], "produces": ["application/json", "application/xml"], "parameters": [{ "in": "body", "name": "body", "description": "Pet object that needs to be added to the store", "required": true, "schema": { "$ref": "#/definitions/Pet" } }], "responses": { "405": { "description": "Invalid input" } }, "security": [{ "petstore_auth": ["write:pets", "read:pets"] }] }, "put": { "tags": ["pet"], "summary": "Update an existing pet", "description": "", "operationId": "updatePet", "consumes": ["application/json", "application/xml"], "produces": ["application/json", "application/xml"], "parameters": [{ "in": "body", "name": "body", "description": "Pet object that needs to be added to the store", "required": true, "schema": { "$ref": "#/definitions/Pet" } }], "responses": { "400": { "description": "Invalid ID supplied" }, "404": { "description": "Pet not found" }, "405": { "description": "Validation exception" } }, "security": [{ "petstore_auth": ["write:pets", "read:pets"] }] } }, "/pet/findByStatus": { "get": { "tags": ["pet"], "summary": "Finds Pets by status", "description": "Multiple status values can be provided with comma separated strings", "operationId": "findPetsByStatus", "produces": ["application/json", "application/xml"], "parameters": [{ "name": "status", "in": "query", "description": "Status values that need to be considered for filter", "required": true, "type": "array", "items": { "type": "string", "enum": ["available", "pending", "sold"], "default": "available" }, "collectionFormat": "multi" }], "responses": { "200": { "description": "successful operation", "schema": { "type": "array", "items": { "$ref": "#/definitions/Pet" } } }, "400": { "description": "Invalid status value" } }, "security": [{ "petstore_auth": ["write:pets", "read:pets"] }] } }, "/pet/findByTags": { "get": { "tags": ["pet"], "summary": "Finds Pets by tags", "description": "Multiple tags can be provided with comma separated strings. Use tag1, tag2, tag3 for testing.", "operationId": "findPetsByTags", "produces": ["application/json", "application/xml"], "parameters": [{ "name": "tags", "in": "query", "description": "Tags to filter by", "required": true, "type": "array", "items": { "type": "string" }, "collectionFormat": "multi" }], "responses": { "200": { "description": "successful operation", "schema": { "type": "array", "items": { "$ref": "#/definitions/Pet" } } }, "400": { "description": "Invalid tag value" } }, "security": [{ "petstore_auth": ["write:pets", "read:pets"] }], "deprecated": true } }, "/pet/{petId}": { "get": { "tags": ["pet"], "summary": "Find pet by ID", "description": "Returns a single pet", "operationId": "getPetById", "produces": ["application/json", "application/xml"], "parameters": [{ "name": "petId", "in": "path", "description": "ID of pet to return", "required": true, "type": "integer", "format": "int64" }], "responses": { "200": { "description": "successful operation", "schema": { "$ref": "#/definitions/Pet" } }, "400": { "description": "Invalid ID supplied" }, "404": { "description": "Pet not found" } }, "security": [{ "api_key": [] }] }, "post": { "tags": ["pet"], "summary": "Updates a pet in the store with form data", "description": "", "operationId": "updatePetWithForm", "consumes": ["application/x-www-form-urlencoded"], "produces": ["application/json", "application/xml"], "parameters": [{ "name": "petId", "in": "path", "description": "ID of pet that needs to be updated", "required": true, "type": "integer", "format": "int64" }, { "name": "name", "in": "formData", "description": "Updated name of the pet", "required": false, "type": "string" }, { "name": "status", "in": "formData", "description": "Updated status of the pet", "required": false, "type": "string" }], "responses": { "405": { "description": "Invalid input" } }, "security": [{ "petstore_auth": ["write:pets", "read:pets"] }] }, "delete": { "tags": ["pet"], "summary": "Deletes a pet", "description": "", "operationId": "deletePet", "produces": ["application/json", "application/xml"], "parameters": [{ "name": "api_key", "in": "header", "required": false, "type": "string" }, { "name": "petId", "in": "path", "description": "Pet id to delete", "required": true, "type": "integer", "format": "int64" }], "responses": { "400": { "description": "Invalid ID supplied" }, "404": { "description": "Pet not found" } }, "security": [{ "petstore_auth": ["write:pets", "read:pets"] }] } }, "/store/inventory": { "get": { "tags": ["store"], "summary": "Returns pet inventories by status", "description": "Returns a map of status codes to quantities", "operationId": "getInventory", "produces": ["application/json"], "parameters": [], "responses": { "200": { "description": "successful operation", "schema": { "type": "object", "additionalProperties": { "type": "integer", "format": "int32" } } } }, "security": [{ "api_key": [] }] } }, "/store/order": { "post": { "tags": ["store"], "summary": "Place an order for a pet", "description": "", "operationId": "placeOrder", "consumes": ["application/json"], "produces": ["application/json", "application/xml"], "parameters": [{ "in": "body", "name": "body", "description": "order placed for purchasing the pet", "required": true, "schema": { "$ref": "#/definitions/Order" } }], "responses": { "200": { "description": "successful operation", "schema": { "$ref": "#/definitions/Order" } }, "400": { "description": "Invalid Order" } } } }, "/store/order/{orderId}": { "get": { "tags": ["store"], "summary": "Find purchase order by ID", "description": "For valid response try integer IDs with value >= 1 and <= 10. Other values will generated exceptions", "operationId": "getOrderById", "produces": ["application/json", "application/xml"], "parameters": [{ "name": "orderId", "in": "path", "description": "ID of pet that needs to be fetched", "required": true, "type": "integer", "maximum": 10, "minimum": 1, "format": "int64" }], "responses": { "200": { "description": "successful operation", "schema": { "$ref": "#/definitions/Order" } }, "400": { "description": "Invalid ID supplied" }, "404": { "description": "Order not found" } } }, "delete": { "tags": ["store"], "summary": "Delete purchase order by ID", "description": "For valid response try integer IDs with positive integer value. Negative or non-integer values will generate API errors", "operationId": "deleteOrder", "produces": ["application/json", "application/xml"], "parameters": [{ "name": "orderId", "in": "path", "description": "ID of the order that needs to be deleted", "required": true, "type": "integer", "minimum": 1, "format": "int64" }], "responses": { "400": { "description": "Invalid ID supplied" }, "404": { "description": "Order not found" } } } }, "/user/createWithList": { "post": { "tags": ["user"], "summary": "Creates list of users with given input array", "description": "", "operationId": "createUsersWithListInput", "consumes": ["application/json"], "produces": ["application/json", "application/xml"], "parameters": [{ "in": "body", "name": "body", "description": "List of user object", "required": true, "schema": { "type": "array", "items": { "$ref": "#/definitions/User" } } }], "responses": { "default": { "description": "successful operation" } } } }, "/user/{username}": { "get": { "tags": ["user"], "summary": "Get user by user name", "description": "", "operationId": "getUserByName", "produces": ["application/json", "application/xml"], "parameters": [{ "name": "username", "in": "path", "description": "The name that needs to be fetched. Use user1 for testing. ", "required": true, "type": "string" }], "responses": { "200": { "description": "successful operation", "schema": { "$ref": "#/definitions/User" } }, "400": { "description": "Invalid username supplied" }, "404": { "description": "User not found" } } }, "put": { "tags": ["user"], "summary": "Updated user", "description": "This can only be done by the logged in user.", "operationId": "updateUser", "consumes": ["application/json"], "produces": ["application/json", "application/xml"], "parameters": [{ "name": "username", "in": "path", "description": "name that need to be updated", "required": true, "type": "string" }, { "in": "body", "name": "body", "description": "Updated user object", "required": true, "schema": { "$ref": "#/definitions/User" } }], "responses": { "400": { "description": "Invalid user supplied" }, "404": { "description": "User not found" } } }, "delete": { "tags": ["user"], "summary": "Delete user", "description": "This can only be done by the logged in user.", "operationId": "deleteUser", "produces": ["application/json", "application/xml"], "parameters": [{ "name": "username", "in": "path", "description": "The name that needs to be deleted", "required": true, "type": "string" }], "responses": { "400": { "description": "Invalid username supplied" }, "404": { "description": "User not found" } } } }, "/user/login": { "get": { "tags": ["user"], "summary": "Logs user into the system", "description": "", "operationId": "loginUser", "produces": ["application/json", "application/xml"], "parameters": [{ "name": "username", "in": "query", "description": "The user name for login", "required": true, "type": "string" }, { "name": "password", "in": "query", "description": "The password for login in clear text", "required": true, "type": "string" }], "responses": { "200": { "description": "successful operation", "headers": { "X-Expires-After": { "type": "string", "format": "date-time", "description": "date in UTC when token expires" }, "X-Rate-Limit": { "type": "integer", "format": "int32", "description": "calls per hour allowed by the user" } }, "schema": { "type": "string" } }, "400": { "description": "Invalid username/password supplied" } } } }, "/user/logout": { "get": { "tags": ["user"], "summary": "Logs out current logged in user session", "description": "", "operationId": "logoutUser", "produces": ["application/json", "application/xml"], "parameters": [], "responses": { "default": { "description": "successful operation" } } } }, "/user/createWithArray": { "post": { "tags": ["user"], "summary": "Creates list of users with given input array", "description": "", "operationId": "createUsersWithArrayInput", "consumes": ["application/json"], "produces": ["application/json", "application/xml"], "parameters": [{ "in": "body", "name": "body", "description": "List of user object", "required": true, "schema": { "type": "array", "items": { "$ref": "#/definitions/User" } } }], "responses": { "default": { "description": "successful operation" } } } }, "/user": { "post": { "tags": ["user"], "summary": "Create user", "description": "This can only be done by the logged in user.", "operationId": "createUser", "consumes": ["application/json"], "produces": ["application/json", "application/xml"], "parameters": [{ "in": "body", "name": "body", "description": "Created user object", "required": true, "schema": { "$ref": "#/definitions/User" } }], "responses": { "default": { "description": "successful operation" } } } } }, "securityDefinitions": { "api_key": { "type": "apiKey", "name": "api_key", "in": "header" }, "petstore_auth": { "type": "oauth2", "authorizationUrl": "https://petstore.swagger.io/oauth/authorize", "flow": "implicit", "scopes": { "read:pets": "read your pets", "write:pets": "modify pets in your account" } } }, "definitions": { "ApiResponse": { "type": "object", "properties": { "code": { "type": "integer", "format": "int32" }, "type": { "type": "string" }, "message": { "type": "string" } } }, "Category": { "type": "object", "properties": { "id": { "type": "integer", "format": "int64" }, "name": { "type": "string" } }, "xml": { "name": "Category" } }, "Pet": { "type": "object", "required": ["name", "photoUrls"], "properties": { "id": { "type": "integer", "format": "int64" }, "category": { "$ref": "#/definitions/Category" }, "name": { "type": "string", "example": "doggie" }, "photoUrls": { "type": "array", "xml": { "wrapped": true }, "items": { "type": "string", "xml": { "name": "photoUrl" } } }, "tags": { "type": "array", "xml": { "wrapped": true }, "items": { "xml": { "name": "tag" }, "$ref": "#/definitions/Tag" } }, "status": { "type": "string", "description": "pet status in the store", "enum": ["available", "pending", "sold"] } }, "xml": { "name": "Pet" } }, "Tag": { "type": "object", "properties": { "id": { "type": "integer", "format": "int64" }, "name": { "type": "string" } }, "xml": { "name": "Tag" } }, "Order": { "type": "object", "properties": { "id": { "type": "integer", "format": "int64" }, "petId": { "type": "integer", "format": "int64" }, "quantity": { "type": "integer", "format": "int32" }, "shipDate": { "type": "string", "format": "date-time" }, "status": { "type": "string", "description": "Order Status", "enum": ["placed", "approved", "delivered"] }, "complete": { "type": "boolean" } }, "xml": { "name": "Order" } }, "User": { "type": "object", "properties": { "id": { "type": "integer", "format": "int64" }, "username": { "type": "string" }, "firstName": { "type": "string" }, "lastName": { "type": "string" }, "email": { "type": "string" }, "password": { "type": "string" }, "phone": { "type": "string" }, "userStatus": { "type": "integer", "format": "int32", "description": "User Status" } }, "xml": { "name": "User" } } }, "externalDocs": { "description": "Find out more about Swagger", "url": "http://swagger.io" } };

/**
 * @type {import("../script.js").OpenApiJson}
 */
const openapiJson = {
    "openapi": "3.1.0",
    "info": {
        "title": "ESP32-CAM-ML",
        "description": "ESP32-CAM-ML API",
        "version": "1.2.0"
    },
    "servers": [
        {
            "url": "http://192.168.69.1:{port}",
            "variables": {
                "port": {
                    "enum": [
                        80,
                        81,
                        3232
                    ],
                    "default": "80"
                }
            }
        }
    ],
    "tags": [
        {
            "name": "App",
            "description": "App API"
        },
        {
            "name": "API",
            "description": "Self API"
        },
        {
            "name": "Settings",
            "description": "Settings API"
        },
        {
            "name": "Sensor",
            "description": "Sensor API"
        },
        {
            "name": "Stream",
            "description": "Stream API"
        },
        {
            "name": "OTA",
            "description": "OTA API"
        }
    ],
    "paths": {
        "/": {
            "get": {
                "tags": [
                    "App"
                ],
                "summary": "Get App",
                "description": "Web App of ESP32-CAM-ML",
                "operationId": "getApp",
                "responses": {
                    "200": {
                        "description": "Open App",
                        "content": {
                            "text/html": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/api": {
            "get": {
                "tags": [
                    "API"
                ],
                "summary": "Get API Docs",
                "description": "Web App API Docs of ESP32-CAM-ML",
                "operationId": "getAPI",
                "responses": {
                    "200": {
                        "description": "Open API Docs",
                        "content": {
                            "text/html": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/api/openapi.json": {
            "get": {
                "tags": [
                    "API"
                ],
                "summary": "Get OpenAPI JSON",
                "description": "Web App OpenAPI JSON Docs of ESP32-CAM-ML",
                "operationId": "getAPIjson",
                "responses": {
                    "200": {
                        "description": "Get OpenAPI JSON Docs",
                        "content": {
                            "application/json": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/settings": {
            "get": {
                "tags": [
                    "Settings"
                ],
                "summary": "Get Settings JSON",
                "description": "Settings JSON of ESP32-CAM-ML",
                "operationId": "getSettings",
                "responses": {
                    "200": {
                        "description": "Get Settings JSON",
                        "content": {
                            "application/json": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    }
                }
            },
            "post": {
                "tags": [
                    "Settings"
                ],
                "summary": "Set Settings JSON",
                "description": "Set Settings JSON of ESP32-CAM-ML",
                "operationId": "setSettings",
                "requestBody": {
                    "content": {
                        "application/json": {
                            "schema": {
                                "$ref": "#/components/schemas/Settings"
                            }
                        }
                    },
                    "required": true
                },
                "responses": {
                    "200": {
                        "description": "Applied Settings JSON",
                        "content": {
                            "application/json": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    },
                    "400": {
                        "description": "Bad Request",
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    },
                    "500": {
                        "description": "Internal Server Error",
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/sensor": {
            "get": {
                "tags": [
                    "Sensor"
                ],
                "summary": "Get Sensor Settings",
                "description": "Sensor Settings of ESP32-CAM-ML",
                "operationId": "getSensorSettings",
                "responses": {
                    "200": {
                        "description": "Sensor Settings",
                        "content": {
                            "application/json": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    },
                    "400": {
                        "description": "Bad Request",
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    }
                }
            },
            "post": {
                "tags": [
                    "Sensor"
                ],
                "summary": "Set Sensor Settings",
                "description": "Set Sensor Settings of ESP32-CAM-ML",
                "operationId": "setSensorSettings",
                "requestBody": {
                    "content": {
                        "application/json": {
                            "schema": {
                                "$ref": "#/components/schemas/Sensor"
                            }
                        }
                    },
                    "required": true
                },
                "responses": {
                    "200": {
                        "description": "Applied Sensor Settings",
                        "content": {
                            "application/json": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    },
                    "400": {
                        "description": "Bad Request",
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "string"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/stream": {
            "get": {
                "tags": [
                    "Stream"
                ],
                "summary": "Camera Stream",
                "description": "Camera Live Stream",
                "operationId": "getStream",
                "responses": {
                    "200": {
                        "description": "Camera Stream",
                        "content": {
                            "multipart/x-mixed-replace": {
                                "schema": {
                                    "type": "string",
                                    "format": "binary"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/ota": {}
    },
    "components": {
        "schemas": {
            "Settings": {
                "type": "object",
                "properties": {}
            },
            "Sensor": {
                "type": "object",
                "properties": {
                    "pixformat": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 8,
                        "format": "int32"
                    },
                    "framesize": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 13,
                        "format": "int32"
                    },
                    "brightness": {
                        "type": "integer",
                        "minimum": -2,
                        "maximum": 2,
                        "format": "int8"
                    },
                    "contrast": {
                        "type": "integer",
                        "minimum": -2,
                        "maximum": 2,
                        "format": "int8"
                    },
                    "saturation": {
                        "type": "integer",
                        "minimum": -2,
                        "maximum": 2,
                        "format": "int8"
                    },
                    "sharpness": {
                        "type": "integer",
                        "minimum": -2,
                        "maximum": 2,
                        "format": "int8"
                    },
                    "denoise": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "gainceiling": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 6,
                        "format": "int8"
                    },
                    "quality": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 63,
                        "format": "uint8"
                    },
                    "colorbar": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "whitebal": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "gain_ctrl": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "exposure_ctrl": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "hmirror": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 1,
                        "format": "uint8"
                    },
                    "vflip": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 1,
                        "format": "uint8"
                    },
                    "aec2": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "awb_gain": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "agc_gain": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 30,
                        "format": "uint8"
                    },
                    "aec_value": {
                        "type": "integer",
                        "format": "uint16"
                    },
                    "special_effect": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 6,
                        "format": "uint8"
                    },
                    "wb_mode": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 4,
                        "format": "uint8"
                    },
                    "ae_level": {
                        "type": "integer",
                        "minimum": -2,
                        "maximum": 2,
                        "format": "int8"
                    },
                    "dcw": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "bpc": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "wpc": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "raw_gma": {
                        "type": "integer",
                        "format": "uint8"
                    },
                    "lenc": {
                        "type": "integer",
                        "format": "uint8"
                    }
                }
            }
        }
    }
};

/**
 * @type {import("../script.js").DeviceSettings}
 */
const settingsConfig = {
    magic: BigInt("72620543991349848"),
    wifi: {
        mode: 2,
        timeout: 25000,
        hostname: "ESP32-Sense",
        security: 4,
        ap: {
            ssid: "ESP32-Sense-AP",
            pass: "TotallySecurePassword1234!",
            local_ip: "192.168.69.1",
            gateway: "192.168.69.1",
            subnet: "255.255.255.0"
        },
        sta: {
            ssid: "",
            pass: "",
            dhcp: false,
            local_ip: "0.0.0.0",
            gateway: "0.0.0.0",
            subnet: "0.0.0.0",
            dns1: "94.140.14.140",
            dns2: "94.140.14.141"
        }
    },
    ota: {
        path: "/update",
        username: "",
        password: ""
    },
    camera: {
        xclk_freq_hz: 20000000,
        ledc_timer: 0,
        ledc_channel: 0,
        pixel_format: 4,
        frame_size: 9,
        jpeg_quality: 12,
        fb_count: 1,
        fb_location: 0,
        grab_mode: 1
    },
    $types: {
        wifi: {
            mode: { $enum: ["NULL", "STA", "AP", "APSTA", "NAN", "MAX"] },
            security: { $enum: ["OPEN", "WEP", "WPA_PSK", "WPA2_PSK", "WPA_WPA2_PSK", "WPA2_ENTERPRISE", "WPA3_PSK", "WPA2_WPA3_PSK", "WAPI_PSK", "OWE", "WPA3_ENT_192", "MAX"] }
        },
        camera: {
            pixel_format: ["RGB565", "YUV422", "YUV420", "GRAYSCALE", "JPEG", "RGB888", "RAW", "RGB444", "RGB555"],
            frame_size: ["96X96", "QQVGA", "QCIF", "HQVGA", "240X240", "QVGA", "CIF", "HVGA", "VGA", "SVGA", "XGA", "HD", "SXGA"],
            fb_location: ["IN_PSRAM", "IN_DRAM"],
            grab_mode: ["WHEN_EMPTY", "LATEST"]
        }
    }
};

/**
 * @type {import("../script.js").SensorSettings}
 */
const sensorConfig = {
    pixformat: 0,
    framesize: 0,
    brightness: 0,
    contrast: 0,
    saturation: 0,
    sharpness: 0,
    denoise: 0,
    special_effect: 0,
    wb_mode: 0,
    awb_gain: 0,
    aec2: 0,
    ae_level: 0,
    aec_value: 0,
    agc_gain: 0,
    gainceiling: 0,
    bpc: 0,
    wpc: 0,
    raw_gma: 0,
    lenc: 0,
    hmirror: 0,
    vflip: 0,
    dcw: 0,
    colorbar: 0,
    quality: 0,
    whitebal: 0,
    gain_ctrl: 0,
    exposure_ctrl: 0
};

QUnit.module("ESP32-CAM-ML Tests", function (hooks) {

    QUnit.test("p5 library import", function (assert) {
        assert.ok(p5, "p5 library is imported correctly");
    });

    QUnit.test("ml5 library import", function (assert) {
        assert.ok(ml5, "ml5 library is imported correctly");
    });

    QUnit.test("Tweakpane library import", function (assert) {
        assert.ok(Tweakpane, "Tweakpane library is imported correctly");
    });

    QUnit.test("ObjectDetector constructor", (assert) => {
        const done = assert.async();

        const model = 'CocoSsd';
        const div = document.createElement('div');
        new p5((p) => {
            /**  @type {p5.MediaElement} */
            let capture;
            p.setup = () => {
                p.createCanvas(640, 480);
                p.noLoop();
                capture = p.createImg(testImage, "Stream", "", async () => {
                    const detector = new ObjectDetector(model, capture);
                    assert.ok(ObjectDetector.detector, 'Detector is initialized');
                    assert.equal(ObjectDetector.model, model, 'Model is set correctly');
                    assert.strictEqual(detector.image, capture, "Image is assigned correctly");

                    if (detector.image)
                        ObjectDetector.detector?.detect(detector.image, (error, results) => {
                            assert.notOk(error, "Error during detection");
                            assert.ok(results.length > 0, "Objects detected");

                            div.remove();
                            capture.remove();

                            done();
                        });
                    else {
                        done();
                    }
                });
            };
        });
    });

    /*
    QUnit.test("resizeVideo", function (assert) {
        const originalSize = { w: 1280, h: 720 };
        const boxSize = { w: 640, h: 480 };

        const scaledSize = resizeVideo(originalSize, boxSize);

        assert.strictEqual(scaledSize.w, 640, "Width is scaled correctly");
        assert.strictEqual(scaledSize.h, 360, "Height is scaled correctly");
    });
    */

    QUnit.module('fetchSettings and sendSettings', (hooks) => {

        QUnit.test('fetchSettings', async (assert) => {
            const settings = /** @type {Object} */ (await fetchSettings(petstoreJsonUrl));
            assert.deepEqual(settings, petstore, 'Settings fetched correctly');
        });

        QUnit.test('sendSettings', async (assert) => {
            const settings = settingsConfig;
            await sendSettings(new URL("https://echo.free.beeceptor.com"), settings);
            assert.ok(true, 'Settings sent correctly');
        });

        QUnit.test('throws fetchSettings', async (assert) => {
            try {
                await fetchSettings(new URL("http://invalid.invalid"));
            } catch (err) {
                assert.throws(() => { throw Error(); });
            }
        });

        QUnit.test('throws sendSettings', async (assert) => {
            try {
                const settings = settingsConfig;
                await sendSettings(new URL("https://example.com"), settings);
            } catch (err) {
                assert.throws(() => { throw Error(); });
            }
        });
    });

    QUnit.module('UI Tests', hooks => {
        hooks.beforeEach(() => {
            this.deviceConfig = settingsConfig;
            this.sensorConfig = sensorConfig;
            this.openapiJson = openapiJson;
            this.elm = document.createElement("div");
            this.elm.id = "tweakpane-container";
            document.body.append(this.elm);
        });

        hooks.afterEach(() => {
            this.elm.remove();
        });

        QUnit.test('UI instance creation', assert => {
            this.ui = new UI(this.deviceConfig, this.sensorConfig, this.openapiJson);
            assert.ok(this.ui, 'UI instance should be created');
            this.ui.destroy;
        });

        QUnit.test('UI createSensorBindings method', assert => {
            this.ui = new UI(this.deviceConfig, this.sensorConfig, this.openapiJson);

            const folder = this.ui.pane.addFolder({ title: 'Test Folder', expanded: true });
            this.ui.createSensorBindings(folder, this.sensorConfig, this.openapiJson);

            assert.ok(folder, 'Sensor bindings should be created');
            this.ui.destroy;
        });

        QUnit.test('UI addDeviceBinding method', assert => {
            this.ui = new UI(this.deviceConfig, this.sensorConfig, this.openapiJson);

            const folder = this.ui.pane.addFolder({ title: 'Test Folder', expanded: true });
            const setting = settingsConfig;
            const types = setting.$types;

            this.ui.addDeviceBinding(folder, setting.wifi, types, 'wifi', 'mode');

            assert.equal(folder.children.length, 1, 'Binding should be added to the folder');
            this.ui.destroy;
        });
    });

});

QUnit.start();