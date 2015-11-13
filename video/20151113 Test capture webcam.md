Code basé sur https://gist.github.com/jayrambhia/5866483

Si le processus program.elf tourne:

    # ./capturev4l2.elf
    Driver Caps:
      Driver: "ispvideo"
      Card: "OMAP3 ISP CSI2a output"
      Bus: "media"
      Version: 0.0
      Capabilities: 04000001
    Querying Cropping Capabilities: Invalid argument
      FMT : CE Desc
    --------------------
    Selected Camera Mode:
      Width: 1280
      Height: 720
      PixFmt: UYVY?
      Field: 1
    Length: 1843200
    Address: 0x402fe000
    Image Length: 0
    Start Capture: Device or resource busy
    # 

Je tue le processus program.elf, le programme donne:

    # ./capturev4l2.elf
    Driver Caps:
      Driver: "ispvideo"
      Card: "OMAP3 ISP CSI2a output"
      Bus: "media"
      Version: 0.0
      Capabilities: 04000001
    Querying Cropping Capabilities: Invalid argument
      FMT : CE Desc
    --------------------
    Selected Camera Mode:
      Width: 1280
      Height: 720
      PixFmt: UYVY?
      Field: 1
    Length: 1843200
    Address: 0x402fe000
    Image Length: 0
    # 

Donc program.elf occupe la webcam.

Paparazzi:
http://docs.paparazziuav.org/latest//ardrone_2board_8c_source.html --> définition structures + init board
http://docs.paparazziuav.org/latest//video__thread_8c_source.html#l00145 --> thread capture
http://docs.paparazziuav.org/latest//v4l2_8c_source.html --> fonctions capture webcam
