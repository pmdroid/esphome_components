# Example configuration for using the water level sensor component

# First, include the external componen
```yaml
external_components:
  - source: github://pmdroid/esphome_components
    components: [water_level_sensor]
```

# Setup I2C bus to be used by the component
```yaml
i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true
```

# Add the water level sensor
```yaml
sensor:
  - platform: water_level_sensor
    name: "Water Tank Level"
    update_interval: 15s
    # Optional: custom I2C addresses if your device uses different ones
    # i2c_low_address: 0x77  # Default: 0x77
    # i2c_high_address: 0x78  # Default: 0x78
```
