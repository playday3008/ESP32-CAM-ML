# Test cases

<details>
  <summary>Example</summary>

  ```md
  ## Test case name
  
  <!-- Choose one of the following: -->
  <span style="color:red">**Failed**</span>
  <span style="color:green">**Passed**</span>
  <span style="color:orange">**Explained**</span>
  
  <span style="font-size:1.25em;">**Brief description**</span>
  
  Briefly describe what you've done.
  
  <span style="font-size:1.25em;">**Steps to reproduce**</span>
  
  Provide a list of steps to reproduce.
  
  <span style="font-size:1.25em;">**Expected behavior**</span>
  
  Describe what you expected to happen.
  
  <span style="font-size:1.25em;">**Actual behavior**</span>
  
  Describe what actually happened. (Could be 'As expected', which means the test passed.)
  
  <span style="font-size:1.25em;">**Environment**</span>
  
  - Browser: [e.g. Chrome, Firefox]
  - OS: [e.g. Windows, macOS, Linux]
  - etc.
  
  <!-- Optional fields -->
  
  <span style="font-size:1.25em;">**Dev Response**</span>
  
  Explanation of the behavior. (Why it's expected/unfixable/etc.)
  
  ```

</details>

## Launching the program in Vivaldi web browser on PC

<span style="color:green">**Passed**</span>

<span style="font-size:1.25em;">**Brief description**</span>

Tested if the program can be launched successfully launched in Vivaldi web browser on PC.

<span style="font-size:1.25em;">**Steps to reproduce**</span>

1. Open Vivaldi web browser on PC.
2. Connect to the device.
3. Type in the IP address of the device in the address bar.
4. Launch the program.

<span style="font-size:1.25em;">**Expected behavior**</span>

The browser should connect to the device and be able to successfully launch the program.

<span style="font-size:1.25em;">**Actual behavior**</span>

The browser behaved as expected.

<span style="font-size:1.25em;">**Environment**</span>

- Browser: Vivaldi
- OS: Windows 11
- Device type: PC

## Resetting the settings to default values

<span style="color:green">**Passed**</span>

<span style="font-size:1.25em;">**Brief description**</span>

After changing the settings, tried to reset them to default values.

<span style="font-size:1.25em;">**Steps to reproduce**</span>

- Connect to the device.
- Launch the program.
- Change the settings to different values.
- Disconnect from the device.
- Reconnect to the device.
- Reset the settings by holding the reset button on device

<span style="font-size:1.25em;">**Expected behavior**</span>

The device should reset the settings to default values after holding the reset button.

<span style="font-size:1.25em;">**Actual behavior**</span>

Device behaved as expected.

<span style="font-size:1.25em;">**Environment**</span>

- Browser: Vivaldi
- OS: Windows 11
- Device type: PC

## Testiong WiFi settings

<span style="color:green">**Passed**</span>

<span style="font-size:1.25em;">**Brief description**</span>

Changing the settings from both Ap and Station categories.

<span style="font-size:1.25em;">**Steps to reproduce**</span>

- Connect to the device.
- Launch the program.
- Change every setting from both Ap and Station categories to a different value one at a time reconnecting to the device after each change.

<span style="font-size:1.25em;">**Expected behavior**</span>

Chaging the settings should affect the IP adress, id and password of the device and allow for the connection to the device with the new settings.

<span style="font-size:1.25em;">**Actual behavior**</span>

Behaved as expected.

<span style="font-size:1.25em;">**Environment**</span>

- Browser: Vivaldi
- OS: Windows 11
- Device type: PC

## Testing Sensor Settings

<span style="color:green">**Passed**</span>

<span style="font-size:1.25em;">**Brief description**</span>

Changing the values of settings categorised as Sensor Settings.

<span style="font-size:1.25em;">**Steps to reproduce**</span>

- Connect to the device.
- Launch the program.
- Change every setting categorised as Sensor Settings to a different value one at a time.

<span style="font-size:1.25em;">**Expected behavior**</span>

The Program should allow the change of each setting to a different value, and continue to function properly without any issues and need to restart.

<span style="font-size:1.25em;">**Actual behavior**</span>

Program behaved as expected.

<span style="font-size:1.25em;">**Environment**</span>

- Browser: Vivaldi
- OS: Windows 11
- Device type: PC

## Changing Pixel_format type

<span style="color:red">**Failed**</span>

<span style="font-size:1.25em;">**Brief description**</span>

Swapping the pixel_format type from default JPEG to other types from drop-down menu.

<span style="font-size:1.25em;">**Steps to reproduce**</span>

- Connect to the device.
- Launch the program.
- Change the pixel_format type to a different one

<span style="font-size:1.25em;">**Expected behavior**</span>

Pixel_format type should change to the selected one without any issues or need to restart and function properly.

<span style="font-size:1.25em;">**Actual behavior**</span>

Types change properly but every type other then the default JPEG either crashes the program or doesn't function properly.

<span style="font-size:1.25em;">**Environment**</span>

- Browser: Vivaldi
- OS: Windows 11
- Device type: PC

## Testing the Frame_size setting

<span style="color:green">**Passed**</span>

<span style="font-size:1.25em;">**Brief description**</span>

Selecting different frame sizes from the drop-down menu.

<span style="font-size:1.25em;">**Steps to reproduce**</span>

- Connect to the device.
- Launch the program.
- Change the frame size to a different one.

<span style="font-size:1.25em;">**Expected behavior**</span>

Frame size should change to the selected one without any issues or need to restart and still function properly.

<span style="font-size:1.25em;">**Actual behavior**</span>

Behaved as expected. (when the device is overheated sometimes there is a need to restart the program for the setting to work properly)

<span style="font-size:1.25em;">**Environment**</span>

- Browser: Vivaldi
- OS: Windows 11
- Device type: PC

## Testing the Jpeg_quality setting

<span style="color:green">**Passed**</span>

<span style="font-size:1.25em;">**Brief description**</span>

Setting the Jpeg_quality setting to different values.

<span style="font-size:1.25em;">**Steps to reproduce**</span>

- Connect to the device.
- Launch the program.
- Set the Jpeg_quality setting to a different value.

<span style="font-size:1.25em;">**Expected behavior**</span>

Jpeg_quality setting should change it's value without any issues or need to restart and still function properly.

<span style="font-size:1.25em;">**Actual behavior**</span>

Behaved as expected.

<span style="font-size:1.25em;">**Environment**</span>

- Browser: Vivaldi
- OS: Windows 11
- Device type: PC
