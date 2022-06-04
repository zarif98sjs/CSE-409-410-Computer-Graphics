/*****************
	 * 
	 * 
	 * SPHERE : 8 part
	 * 
	 * 
	 *****************/

	glColor3f(1,0,0);

	// z : up , x,y : all combination

    // UP 1
    glPushMatrix();
	{
		glTranslatef(objH+objR/2,objH+objR/2,objH+objR/2);
    	drawSphere(objR,numSegments,numSegments);
	}
    glPopMatrix();

	// UP 2
    glPushMatrix();
	{
		glTranslatef(objH+objR/2,-objH-objR/2,objH+objR/2);
    	drawSphere(objR,numSegments,numSegments);
	}
    glPopMatrix();

	// UP 3
    glPushMatrix();
	{
		glTranslatef(-objH-objR/2,objH+objR/2,objH+objR/2);
    	drawSphere(objR,numSegments,numSegments);
	}
    glPopMatrix();

	// UP 4
    glPushMatrix();
	{
		glTranslatef(-objH-objR/2,-objH-objR/2,objH+objR/2);
    	drawSphere(objR,numSegments,numSegments);
	}
    glPopMatrix();

	// z : down , x,y : all combination

	// DOWN 1
    glPushMatrix();
	{
		glTranslatef(objH+objR/2,objH+objR/2,-objH-objR/2);
    	drawSphere(objR,numSegments,numSegments);
	}
    glPopMatrix();

	// DOWN 2
    glPushMatrix();
	{
		glTranslatef(objH+objR/2,-objH-objR/2,-objH-objR/2);
    	drawSphere(objR,numSegments,numSegments);
	}
    glPopMatrix();

	// DOWN 3
    glPushMatrix();
	{
		glTranslatef(-objH-objR/2,objH+objR/2,-objH-objR/2);
    	drawSphere(objR,numSegments,numSegments);
	}
    glPopMatrix();

	// DOWN 4
    glPushMatrix();
	{
		glTranslatef(-objH-objR/2,-objH-objR/2,-objH-objR/2);
    	drawSphere(objR,numSegments,numSegments);
	}
    glPopMatrix();