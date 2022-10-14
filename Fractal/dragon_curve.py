## Dragon Curve

import turtle

def dragon_curve(length, turn, depth): 
    if depth == 0:
        turtle.forward(length)
    else:
        if turn == 1:
            turtle.right(45)
            dragon_curve(length, 0, depth-1)
            turtle.left(90)
            dragon_curve(length, 1, depth-1)
            turtle.right(45)
        else:
            turtle.left(45)
            dragon_curve(length, 1, depth-1)
            turtle.right(90)
            dragon_curve(length, 0, depth-1)
            turtle.left(45)
        

def main():
    turtle.setup(800, 600)
    turtle.penup()
    turtle.goto(-250, 150)
    turtle.pendown()
    turtle.pensize(2)
    turtle.speed(0)
    length = 10
    depth = 12 ## Change Depth
    dragon_curve(length, 1, depth)
    turtle.hideturtle()
    turtle.done()

if __name__ == "__main__":
    main()