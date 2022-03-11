# Motorized-orientation-of-venetian-blinds
Automate the orientation of the venetian blinds based on the light or as the user wishes

SCOPE:

The scope of our project was to create an automated motor that orients the venetian blinds by itself or with a wireless communication from the user in order to avoid the bother of continue get up to change manually the orientation during the day.
In order to do that we used an arduino with some sensors.

WORKING SCHEME:
With the arduino board we used:
-a stepper motor that spins the stick that orientates the venetian blinds based on the light intensity and not only fully opened or fully closed
-a photoresistor that allow us to measure the intensity of the light thanks to an analogical read of the tension on the photoresistor.
-a bluetooth module that allows to manually change the orientation of the blinds through a wireless connection.

Through a switch the user can decide if the system will be in “user-mode” or automatic.
During the “user-mode” he is allowed to send the position that the blind will assume immediately thanks the stepper motor.
Otherwise during the automatic-mode the system will read through the photoresistor the intensity of the light and adjust the orientation accordingly every period previously chosen. 

SYSTEM WORKFLOW:

the major blocks of the software are :
the light reader block that we use to read the voltage difference. After that we use a translation function to get lux, (lumens per surface of the photoresistor in our case) that is the unit of measure of the light intensity, from the value that we read.
the movement manager block allow the calculation, with the translation from luxes to steps function,  of the steps that the stepper motor has to do based on the luxes previously calculated and to send the command to the motor. Since the Venetian blind would have the possibility of being closed in 2 directions of rotation, we have chosen only one, since it does not matter which side we close. Thanks to that the steps range is 0 (opened) to -5120 (closed).
the bluetooth block that allows the user to send the exact position that he wants the blinds to be. Since the bluetooth module sends a flow of characters to the system, the microcontroller has to gather all this characters and convert them into a string and then to a decimal number, in order to do that we created a specific function that translates this char flow to a decimal number. The bluetooth block also contains the override manager part that at every iteration it checks if the user wants to enter user-mode or stay in the automatic-mode. 


TESTING:
To set the right parameters for the function that translates from lux to steps we had to run some tests manually in order to create a linear function that allowed us to find this coefficient for the conversion.

Another problem was to find a light source that could change its intensity quickly in order to avoid a daylong test. Initially we decided to add some shields near the photoresistor, but then to test the limits of our system and in particular the range limits we decided to use a dimmable warm light because the now common led light couldn’t be read by the photoresistor (cold light).
We also tried to add interrupts to our project. In particular after the press of a button the system should have called an interrupt handler that had the task to reset the system and the blinds to the initial state, but we have found some problems that we couldn’t solve with easy debugging.For example when the button was pressed the whole system stopped without any reasons.

CONCLUSION:
 
To improve our project we would fix the problems of the interrupt button and add some other features like a little bit of machine learning to train the system to orient the blinds on the choices that the user makes in the training session.
Moreover we could swap the photoresistor with a light sensor in order to make the system more accurate and sensitive to the cold light too because as we said before the photoresistor is not the best choice for this kind of project and also because as we said before it only senses warm lights .

<a title="LINK" href="https://docs.google.com/presentation/d/1_qU5ETJd3HPaiEGd4Cfg13uD3JHw4tIzg7xWpKMwRN8/edit?usp=sharing" target="_blank" > PowerPoint presentation
 <\br>
<a title="LINK" href="https://www.youtube.com/watch?v=DqiI_04X7rQ" target="_blank" > Video demonstration
 
