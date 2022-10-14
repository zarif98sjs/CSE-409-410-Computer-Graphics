## Koch Curve

import turtle

def koch_curve(length, depth):
    if depth == 0:
        turtle.forward(length)
    else:
        koch_curve(length/3, depth-1)
        turtle.left(60)
        koch_curve(length/3, depth-1)
        turtle.right(120)
        koch_curve(length/3, depth-1)
        turtle.left(60)
        koch_curve(length/3, depth-1)
    
def main():
    turtle.setup(800, 600)
    turtle.penup()
    turtle.goto(-300, -50)
    turtle.pendown()
    turtle.pensize(2)
    turtle.speed(0)
    length = 600
    depth = 4 ## Change Depth
    koch_curve(length, depth)
    turtle.hideturtle()
    turtle.done()

if __name__ == "__main__":
    main()