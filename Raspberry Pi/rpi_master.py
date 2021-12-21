####################################
## Raspberry Pi -- master control ##
############################################################################################
## Three-state FSM:                                                                       ##
## 1.Start menu:                                                                          ##
##   press to start, show history highest score, enter demo mode, change player name      ##
## 2.Playing game                                                                         ##
##   output direction signal,                                                             ##
##   press pause, wait for hitting the wall/Apple                                         ##
## 3.Game over                                                                            ##
##   show the score, back to menu                                                         ##
############################################################################################

import serial
from time import sleep
import subprocess
import time
import RPi.GPIO as GPIO
import sys
import pygame
import os
from pygame.locals import *

#------------Global state-related variables-----------------
global state
# state = 0 -> exit game
# state = 1 -> start menu
# state = 2 -> game playing
# state = 3 -> game over
state = 1
global palyer_num
player_name = 'PLAYER'
global direction
# direction = 0 -> Forward    GPIO26  Serial_send '0' 
# direction = 1 -> LEFT       GPIO19  Serial_send '1' 
# direction = 2 -> UP         GPIO23  Serial_send '2' 
# direction = 3 -> Backward   GPIO27  Serial_send '3' 
# direction = 4 -> RIGHT      GPIO17  Serial_send '4' 
# direction = 5 -> DOWN       GPIO22  Serial_send '5' 
direction = 1
global score
score = 0




#------------initialize serial port-----------------
ser = serial.Serial ("/dev/ttyAMA1", 9600)    #Open port with baud rate
#------------initialize touch screen----------------
os.putenv('SDL_VIDEODRIVER','fbcon')
os.putenv('SDL_FBDEV','/dev/fb1')
os.putenv('SDL_MOUSEDRV','TSLIB')
os.putenv('SDL_MOUSEDEV','/dev/input/touchscreen')
#------------GPIO settings--------------------------
GPIO.setmode(GPIO.BCM)   # Set for broadcom numbering not board numbers...
GPIO.setup(19, GPIO.IN, pull_up_down=GPIO.PUD_UP) # LEFT
GPIO.setup(26, GPIO.IN, pull_up_down=GPIO.PUD_UP) # Forward
GPIO.setup(27, GPIO.IN, pull_up_down=GPIO.PUD_UP) # Backward
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_UP) # UP
GPIO.setup(22, GPIO.IN, pull_up_down=GPIO.PUD_UP) # DOWN
GPIO.setup(17, GPIO.IN, pull_up_down=GPIO.PUD_UP) # RIGHT 

def gpio19_callback(channel):   #  callback for gpio 19
        global direction            
        if(direction != 4):       # check the current state is not RIGHT
            print ("Change Direction to LEFT")   # change stage to LEFT
            ser.write('1') 
            direction = 1; 

def gpio26_callback(channel):   #  callback for gpio 26
        global direction             
        if(direction != 3):       # check the current state is not Backward
            print ("Change Direction to FORWARD")   # change stage to Forward
            ser.write('0') 
            direction = 0; 

def gpio27_callback(channel):   #  callback for gpio 27
        global direction, player_name    
        if(state == 2):         
            if(direction != 0):       # check the current state is not Forward
                print ("Change Direction to Backward")   # change stage to Backward
                ser.write('3') 
                direction = 3; 
        if(state == 4):
            player_name= player_name[:-1]
def gpio23_callback(channel):   #  callback for gpio 23
        global direction, player_name    
        if(state == 2):         
            if(direction != 5):       # check the current state is not DOWN
                print ("Change Direction to UP")   # change stage UP
                ser.write('2') 
                direction = 2; 
        if(state == 4):
            length = len(player_name)
            if(player_name[length-1]=='Z' or player_name[length-1]=='_'):
                player_name = player_name[:-1]+'A'
            else:
                player_name = player_name[:-1]+chr(ord(player_name[length-1])+1)

def gpio22_callback(channel):   #  callback for gpio 22
        global direction, player_name, state            
        if(state == 2):
            if(direction != 2):       # check the current state is not UP
                print ("Change Direction to DOWN")   # change stage to DOWN
                ser.write('5') 
                direction = 5
        if(state == 4):
            player_name += 'A'
def gpio17_callback(channel):   #  callback for gpio 17
        global direction, state, player_name             
        if(state == 2):
            if(direction != 1):       # check the current state is not LEFT
                print ("Change Direction to Y Negative")   # change stage to RIGHT
                ser.write('4') 
                direction = 4; 
        if(state == 4):
            length = len(player_name)
            if(player_name[length-1]=='A' or player_name[length-1]=='_'):
                player_name = player_name[:-1]+'Z'
            else:
                player_name = player_name[:-1]+chr(ord(player_name[length-1])-1)

GPIO.add_event_detect(19, GPIO.FALLING, callback=gpio19_callback, bouncetime=400)
GPIO.add_event_detect(22, GPIO.FALLING, callback=gpio22_callback, bouncetime=400)
GPIO.add_event_detect(23, GPIO.FALLING, callback=gpio23_callback, bouncetime=400)
GPIO.add_event_detect(26, GPIO.FALLING, callback=gpio26_callback, bouncetime=400)
GPIO.add_event_detect(27, GPIO.FALLING, callback=gpio27_callback, bouncetime=400)
GPIO.add_event_detect(17, GPIO.FALLING, callback=gpio17_callback, bouncetime=400)
#-----------Initialize Pygame------------------------
pygame.init()
pygame.mouse.set_visible(False)
WHITE = 255,255,255
BLACK = 0,0,0
screen = pygame.display.set_mode((320,240))

#------------buttons setup---------------------------
my_font = pygame.font.Font(None,20)


#------------open scoreboard files-------------------
f = open("/home/pi/final_project/ScoreBoard.txt" , "r")
lines = []
for line in f:
    lines.append(str.split(line))
f.close()
#------------scoreboard player number and score------
global top1name, top1score, top2name, top2score, top3name, top3score 
top1name  = lines[0][0]
top1score = lines[0][1]
top2name  = lines[1][0]
top2score = lines[1][1]
top3name  = lines[2][0]
top3score = lines[2][1]

#--------------------Start Menu----------------------------------------
def start_menu():
    sleep(0.5)
    # load images
    snake = pygame.image.load("/home/pi/final_project/snake.png")
    snake = pygame.transform.scale(snake, (150, 150))
    snakerect = snake.get_rect()
    snakerect = snakerect.move((160, 10))
    title = pygame.image.load("/home/pi/final_project/Title.png")
    title = pygame.transform.scale(title, (150, 40))
    titlerect = title.get_rect()
    titlerect = titlerect.move((0, 0))
    global state
    while (state == 1):
        time.sleep(0.1)
        # Touch response for buttons on the PiTFT
        for event in pygame.event.get():
            if(event.type is MOUSEBUTTONUP):
                pos = pygame.mouse.get_pos()
                x,y = pos
                if y>140:
                    if x<85:
                        print ("begin to play")
                        ser.write('S') 
                        state=2  
                    elif x<175:
                        print("Name Changing Mode") 
                        state=4                    
                    elif x<255:
                        print ("DEMO")
                        state=5
                        ser.write('D') 
                    else:
                        print ("quit pressed, quit")
                        ser.write('R') 
                        state=0
                
                screen.fill(BLACK)
        # Display Buttons
        circle_surface = pygame.display.set_mode((320,240))        
        pygame.draw.circle(circle_surface,(0,255,0),(40,200),30)
        my_start = {'START':(40,200), 'Score Board':(60,60),  'QUIT':(290,200), 'DEMO':(220,200), 'PLAYER NAME':(130,200)}
        score_dyn = { (50,90):top1name,(100,90):str(top1score),(50,110):top2name,(100,110):str(top2score),(50,130):top3name,(100,130):str(top3score)}

        for my_text, text_pos in my_start.items():
            text_surface = my_font.render(my_text, True, WHITE)
            rect = text_surface.get_rect(center=text_pos)
            screen.blit(text_surface, rect)
        for  text_pos, my_text in score_dyn.items():
            text_surface = my_font.render(my_text, True, WHITE)
            rect = text_surface.get_rect(center=text_pos)
            screen.blit(text_surface, rect)
        screen.blit(snake, snakerect)
        screen.blit(title, titlerect)
        pygame.display.flip()

# A helper function converts the state signal to string       
def get_direct_str():
    # direction = 0 -> Forward
    # direction = 1 -> LEFT
    # direction = 2 -> UP
    # direction = 3 -> Backward
    # direction = 4 -> RIGHT
    # direction = 5 -> DOWN
    global direction
    if(direction == 0):
        return 'Forward'
    elif(direction == 1):
        return 'LEFT'
    elif(direction == 2):
        return 'UP'
    elif(direction == 3):
        return 'Backward'
    elif(direction == 4):
        return 'RIGHT'
    elif(direction == 5):
        return 'DOWN'


#---------------Playing Name state-----------------------
def playing_game():
    global state
    global score
    global direction
    
    while(state == 2):
        # display fonts
        playing_font = pygame.font.Font(None,30)
        screen.fill(BLACK)
        time.sleep(0.1)
        score_str = str(score)
        direc_str = get_direct_str()
        my_play = { 'Current Score:':(90,60),  'QUIT':(290,200), 'Current Direction:':(100, 140)}
        my_score_direct = {score_str : (260,60), direc_str: (260,140)}
        for my_text, text_pos in my_play.items():
                text_surface = playing_font.render(my_text, True, WHITE)
                rect = text_surface.get_rect(center=text_pos)
                screen.blit(text_surface, rect)
        for my_text, text_pos  in my_score_direct.items():
                text_surface = playing_font.render(my_text, True, WHITE)
                rect = text_surface.get_rect(center=text_pos)
                screen.blit(text_surface, rect)
        pygame.display.flip()
        # Detect Quit button
        for event in pygame.event.get():
            if(event.type is MOUSEBUTTONUP):
                pos = pygame.mouse.get_pos()
                x,y = pos
                if y>140:
                    if x>190:
                        print ("quit pressed, quit")
                        ser.write('R') 
                        state=0
        # Read serial port sent by Arduino
        while ser.inWaiting():
            received_data = ser.read()
            if received_data == '!':
                print("Game Over")
                state = 3
            if received_data == '+':
                print("score++")
                score += 1

#--------------------Game Over State-----------------------------
def game_over():
    global state
    global score
    global top1name, top1score, top2name, top2score, top3name, top3score, player_name
    
    while(state == 3):
        # Load image
        over = pygame.image.load("/home/pi/final_project/GameOver.jpg")
        over = pygame.transform.scale(over, (220, 80))
        overrect = over.get_rect()
        overrect = overrect.move((60, 0))
        screen.fill(BLACK)
        time.sleep(0.1)
        # Display Fonts
        score_str = str(score)
        direc_str = get_direct_str()
        my_play = { 'You Are:':(92,120), 'QUIT':(290,200), 'Your Score:':(80, 140), 'BACK TO MENU': (60,200)}
        my_score_direct = {str(player_name) : (180,120), score_str: (180,140)}
        for my_text, text_pos in my_play.items():
                text_surface = my_font.render(my_text, True, WHITE)
                rect = text_surface.get_rect(center=text_pos)
                screen.blit(text_surface, rect)
        for my_text, text_pos  in my_score_direct.items():
                text_surface = my_font.render(my_text, True, WHITE)
                rect = text_surface.get_rect(center=text_pos)
                screen.blit(text_surface, rect)
        screen.blit(over, overrect)
        pygame.display.flip()
        # Detect touch buttons at PiTFT
        for event in pygame.event.get():
            if(event.type is MOUSEBUTTONUP):
                pos = pygame.mouse.get_pos()
                x,y = pos
                if y>140:
                    if x>190:
                        print ("quit pressed, quit")
                        ser.write('R') 
                        state=0
                if y>140:
                    if x<100:
                        # reset the LED cube
                        ser.write('R') 
                        # roll the scoreboard if needed
                        if(score >=int(top1score)):
                            print("Change!!!!!")
                            top3name = top2name
                            top3score = top2score
                            top2name = top1name
                            top2score = top1score
                            top1name = player_name
                            top1score = score
                        elif(score >= int(top2score)):
                            top3name = top2name
                            top3score = top2score
                            top2name = player_name
                            top2score = score
                        elif(score >= int(top3score)):
                            top3name = player_name
                            top3score = score
                        # Write the text file
                        f = open("/home/pi/final_project/ScoreBoard.txt", "w")
                        f.write(top1name+' ')
                        f.write(str(top1score)+'\n')
                        f.write(top2name  +' ')
                        f.write(str(top2score)+'\n')
                        f.write(top3name  +' ')
                        f.write(str(top3score) +'\n')    
                        f.close()
                        # back to menu
                        state= 1
                        score = 0
                        print ("Back to menu")

# Change name state
def change_name():
    global state, player_name
    if(state == 4):
        time.sleep(0.1)
        # detect the back button on the PiTFT
        for event in pygame.event.get():
            if(event.type is MOUSEBUTTONUP):
                pos = pygame.mouse.get_pos()
                x,y = pos
                if y>180:
                    if x<85:
                        print ("Back to Start Menu")
                        state=1
                
                screen.fill(BLACK)
        # Display fonts
        circle_surface = pygame.display.set_mode((320,240))        
        pygame.draw.circle(circle_surface,(255,0,0),(40,200),30)
        my_start = {'Confirm':(40,200)}
        instructions = {'Previous Letter ->':(250,40),'Enter >':(280,100),'Next Letter ->':(270,160),'Delete ->':(270,220) }
        Instruction_name = {'Please Enter Your Name Below' :(100,90), ':':(10,110)}
        Entered_Name = {player_name:(90,110)}

        for my_text, text_pos in Instruction_name.items():
            text_surface = my_font.render(my_text, True, WHITE)
            rect = text_surface.get_rect(center=text_pos)
            screen.blit(text_surface, rect)
        for my_text, text_pos in my_start.items():
            text_surface = my_font.render(my_text, True, WHITE)
            rect = text_surface.get_rect(center=text_pos)
            screen.blit(text_surface, rect)
        for my_text, text_pos in instructions.items():
            text_surface = my_font.render(my_text, True, WHITE)
            rect = text_surface.get_rect(center=text_pos)
            screen.blit(text_surface, rect)
        for my_text, text_pos in Entered_Name.items():
            text_surface = my_font.render(my_text, True, WHITE)
            rect = text_surface.get_rect(center=text_pos)
            screen.blit(text_surface, rect)   

        pygame.display.flip()
        
#-----------Demo State--------------------------
def demo_mode():
    global state, player_name

    if(state == 5):
        demo_font = pygame.font.Font(None,30)
        screen.fill(BLACK)
        time.sleep(0.1)
        # detect the button on the screen
        for event in pygame.event.get():
            if(event.type is MOUSEBUTTONUP):
                pos = pygame.mouse.get_pos()
                x,y = pos
                if y>180:
                    if x<85:
                        print ("Back to Start Menu")
                        ser.write('R') 
                        state=1 
        # display fonts
        circle_surface = pygame.display.set_mode((320,240))        
        pygame.draw.circle(circle_surface,(255,0,0),(40,200),30)
        my_start = {'Back':(40,200),'Please Watch the Cube' :(160,60),'Demo In Progress' :(160,100)}
        
        for my_text, text_pos in my_start.items():
            text_surface = demo_font.render(my_text, True, WHITE)
            rect = text_surface.get_rect(center=text_pos)
            screen.blit(text_surface, rect)
        pygame.display.flip()

#--------------Main Function---------------------------
while(state):
    sleep(0.1)
    if (state == 1):
        start_menu()
    if (state == 2):
        playing_game()
    if (state == 3):
        game_over()
    if (state == 4):
        change_name()
    if (state == 5):
        demo_mode()   

GPIO.cleanup()
