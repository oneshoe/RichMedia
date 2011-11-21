
Rich Media was a proof-of-concept project to allow importing MediaSite exports
and present them in your own layout, and use MediaMosa as backend video storage
for lecture recordings. This proof-of-concept was developed for SURFnet
(http://www.surfnet.nl) by One Shoe labs (http://www.oneshoe.nl) in 2010.

There are no plans for further development of this proof-of-concept for a newer
and more future proof version is in development and about to be released. That
project contains lessons learned from this proof-of-concept focusses more on
modern techniques (HTML5 video, CSS3) and has a wider functional spectrum
allowing anything to be added to the video timeline. Keep an eye on our website
http://www.oneshoe.nl and http://www.mediamosa.org for more information.

This repository contains folders "modules", "themes" and "profiles".

Install instructions:

1. Download (and not yet install) the latest Drupal 6 available from
   http://drupal.org/project/drupal

2. Copy the contents of the "modules" folder into sites/all/modules of your
   Drupal code base.

3. Copy the "surfnet_richmedia" folder from "profiles" into the "profiles"
   directory of your Drupal file base.

4. Copy the "richmedia" folder from "themes" into the sites/all/themes folder
   of your Drupal code base.

5. Download a 4.x version of the JW player from
   http://www.longtailvideo.com/players/jw-flv-player/ and install it into the
   sites/all/themes/richmedia/jw/ folder so that player-licenced.swf is inside
   that folder. We've tested the Rich Media functionality with a licensed
   4.5.230 version of the JW player. If you use an unlicenced (non-commercial)
   version, you might need to change the path to the player in the template.php
   at sites/all/themes/richmedia/template.php inside the function
   richmedia_mediaplayer().

6. Open a browser and type the address to your Drupal 6 codebase. That should
   automatically redirect you to the install.php file where you can choose the
   install profile "SURFnet RichMedia" which will guide you through the steps
   of installing all required modules and preconfigured menu items and such.
