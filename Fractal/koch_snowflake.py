## Koch Snowflake

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

def koche_snowflake(length, depth):
    for i in range(3):
        koch_curve(length, depth)
        turtle.right(120)
    
def main():
    turtle.setup(1200, 800)
    turtle.penup()
    turtle.goto(-350, 175)
    turtle.pendown()
    turtle.pensize(2)
    turtle.speed(0)
    length = 600
    depth = 4 ## Change Depth
    koche_snowflake(length, depth)
    turtle.hideturtle()
    turtle.done()

if __name__ == "__main__":
    main()