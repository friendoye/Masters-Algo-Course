import json

def gen_default_configuration(json_output_name):
    configuration_json = {
        "configuration": {
            "predator_time_before_die": 3,
            "predator_increasing_interval": 3,
            "prey_increasing_interval": 3,
            "ocean_width": 5,
            "ocean_height": 5
        },
        "map": {
            "predators": [
                {"x": 0, "y": 0},
                {"x": 4, "y": 4},
                {"x": 0, "y": 4},
                {"x": 4, "y": 0},
                {"x": 2, "y": 2}
            ],
            "preys": [
                {"x": 1, "y": 1},
                {"x": 3, "y": 3},
                {"x": 1, "y": 3},
                {"x": 3, "y": 1}
            ],
            "obstacles": [
                {"x": 1, "y": 0}, {"x": 2, "y": 0}, {"x": 3, "y": 0},
                {"x": 1, "y": 4}, {"x": 2, "y": 4}, {"x": 3, "y": 4},
                {"x": 0, "y": 1}, {"x": 0, "y": 2}, {"x": 0, "y": 3},
                {"x": 4, "y": 1}, {"x": 4, "y": 2}, {"x": 4, "y": 3}
            ]
        }
    }

    with open(json_output_name, "w") as json_output:
        json_output.write(json.dumps(configuration_json, indent=4, sort_keys=True))

if __name__ == "__main__":
    gen_default_configuration("configuration.json")