# Centicrawler

Current Developer: dulsi@identicalsoftware.com

## Made with SFML 2.6 and C++17

- configure: `cmake -B build/`
- compile:   `cmake --build build/`
- run: `./build/bin/centipede`

SMFL dependency must be installed before building.

Debian: `sudo apt install build-essential cmake git libsfml-dev`

Fedora: `sudo dnf install gcc-c++ cmake git SFML-devel`

## Details
The centipede it going to destroy the fairy glade. Other bugs like spiders aid the invaders. You are a fairy tasked with stopping the
threat. Use your magic to transform the centipede into mushroom. Beware the centipede can split into multiple threats if you hit it
in the middle.

## History
Centicrawler was originally designed as a class assignment by Jackson Miller. It was intended to be a clone of Centipede. It used
the original sprite sheet from the 1981 arcade game. Only a minimal set of features were implemented to meet the grading rubric.

The sprites have been replaced with new graphics. Some elements from Centipede are still missing.

Centicrawler is adding new elements like a 2 player simultaneous mode.

Big thanks to these blog posts by the RetroGame Deconstruction Zone:
- [Character Breakdown](https://www.retrogamedeconstructionzone.com/2020/08/the-characters-of-centipede.html)
- [Descent Mechanics](https://www.retrogamedeconstructionzone.com/2020/11/the-descent-of-centipede-part-1.html)

## Missing Features
- Change the laser system to that of the original game (only one laser on screen at a time)
- Flea (drop mushrooms)
- Scorpion (poisons mushrooms)

## Sprite License

### mushroom.png
Mushrooms  
Created by [SCaydi](https://opengameart.org/content/mushroom-4), hosted by OpenGameArt.org  
Modified colors and size by Dennis Payne  
License: CC-BY 3.0, CC-BY 4.0, GPL-3.0  

### spider.png
Spider  
Created by Dennis Payne  

### centipede.png
Centipede  
Created by Dennis Payne  

### fairy.png
Fairy  
Created by [sqbr](https://opengameart.org/content/fairy-princesses), GrafxKid, and cabbit as Svetlana Kushnariova with email address lana-chan@yandex.ru, hosted by OpenGameArt.org  
Modified by Dennis Payne  
License: CC-BY 4.0  

### font.png
Intrepid Monochrome 8-bit Font  
[Font "Intrepid Monochrome"](https://opengameart.org/content/intrepid-monochrome-8-bit-font) by Hammer Forged Games  
Scaled up 4 times original size  
License: CC0  
