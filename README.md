# U--vest
A vest with ultrasonic sensors to aid visually impaired individuals
The project consists of two parts, which are the ultrasonic distance detection system and the object detection system


The ultrasonic range detection system consists of four ultrasonic sensors, each paired with a vibration motor.
The vibration motors vibrate independently of each other, with the vibration frequency proportional to the distance data received from the ultrasonic sensor.
The code for the ultrasonic detection system can be found in the 'vest_main_array' folder


The object detection system consists of an object detection model and a Text-To-Speech(TTS) model 
The object detection model identifies the object in front of the user and outputs it to the TTS model, which then informs the user of the object's identity via an earpiece.
The 'models' folder contains the various edge impulse models for different devices.

EDGE IMPULSE MODELS
https://studio.edgeimpulse.com/public/649207/live
https://studio.edgeimpulse.com/public/652076/live
https://studio.edgeimpulse.com/public/655559/live
https://studio.edgeimpulse.com/public/696756/live - has the highest accuracy
https://studio.edgeimpulse.com/public/700716/live
https://studio.edgeimpulse.com/public/711422/live
https://studio.edgeimpulse.com/public/714674/live

DATASET FOR EDGE IMPULSE MODELS
https://drive.google.com/drive/folders/1e-7eUr8Vi1iK3WYlp5NzejV0hsvuliJK?usp=drive_link
