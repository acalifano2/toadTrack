#
# Author: Armando Califano
# cc Armando Califano
# Purpose: Script to run toadTrack program automating which folder to grab
#	   images from. File path in line 14 can be changed to accomodate
#	   other file paths. An example executable will read:
#		.toadTrack /home/User/toadImages/depthframes0.bmp
#
#
echo "Running BG FRAMES"
#
#
DIR=${1}
NUM=${2}
echo "USING ../Documents/${DIR}/depthframe${NUM}.bmp"
./toadTrack /home/acalifano2/Documents/${DIR}/depthframe${NUM}.bmp
echo DONE
#
