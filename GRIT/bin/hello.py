import pygrit as GRIT
import numpy as np
import math

def main():
	print('My hello pybind\n')

	cfg_filename = 'example.cfg'

	settings = GRIT.ConfigFile()
	settings.load(cfg_filename)

	txt_filename = settings.get_value('txt_filename', 'circle_enright.txt')

	parameters = GRIT.make_parameters_from_config_file(settings)

	engine = GRIT.Engine2D()

	GRIT.init_engine_with_mesh_file(
						  GRIT.get_data_file_path(txt_filename)
						  , parameters
						  , engine
						  )

if __name__ == "__main__":
	main()
