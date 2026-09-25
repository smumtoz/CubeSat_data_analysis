# Pixie

Firmware, telemetry, notebooks, and research materials for the Pixie sensor project.

## Contents

- `pixie_final.ino`: STM32/Arduino firmware that reads pressure, temperature, magnetic, gyroscope, and acceleration sensors, with serial communication and SD-card logging.
- `pixie/`: another firmware copy, a trajectory notebook, and the original `pixie_data.log` telemetry file.
- `trajectroy.ipynb`: notebook with serial-reading and plotting code.
- `pixie_data_with_time.csv`: telemetry data with time columns.
- `UniSat_3_0_research_pixie.pdf`: project research document.
- `Before launch.zip`: six project photographs.

## Working with the project

The firmware requires a compatible STM32 board setup and the sensor libraries referenced by its includes: MS5611, LM75A, LIS3MDL, STM32SD, LSM6DS3, OneWire, and DallasTemperature. It also uses the `IntroStratLib` namespace. Configure the board and verify the pin assignments for your hardware before compiling or uploading.

Open the notebooks in Jupyter with `pyserial` and `matplotlib` available. The serial-reading cells currently use `COM6` at 9600 baud; adjust the port for your device before running those cells.

Notebook checkpoints and the bundled Quartus installer are excluded from this repository. Telemetry files, including `pixie/pixie_data.log`, are retained.
