# Setup

1. Copy config.env.example to config.env
2. Edit parameters:
        image_name - file name (default - image.jpg)
        image_folder - folder in which the file is placed - default is the project's root
        kernel_size - odd size between [3, 11]
        num_levels - up to 8
        size_crop - use to crop the image (around the center), -1 for no cropping
        show_figures - use false to disable image and pyramid levels display
3. Build and run
