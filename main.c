#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct
{
    float real;
    float img;
} Complex;
float Magnitude (Complex a) // function that calculates magnitude of a complex number
{
    float mag = sqrt(pow(a.real, 2) + pow(a.img, 2));
    return mag;
} // My edit
Complex add_Complex (Complex a,Complex b)//A - Function that adds two complex numbers
{
    Complex c;
    c.real = a.real + b.real; // adds real component of b to a
    c.img = a.img + b.img; // adds imaginary component of b to a
    return c;
}
void print_Complex (Complex a)//B - Function that prints a complex number
{
    if (a.img >= 0)
        printf("The complex number is %.2f + %.2fi\n",a.real, a.img); // prints function as a + bi as long as b is nonnegative
    else
        printf("The complex number is %.2f - %.2fi\n",a.real, fabs(a.img)); // writes function as a - bi if b is negative
}
Complex read_Complex () //C - Function that reads a complex number from the user
{
    Complex a;
    printf("Enter a(real part):");
    scanf("%f", &a.real);
    printf("Enter b(imaginary part):");
    scanf("%f", &a.img);
    return a;
}
int Compare_Complex (Complex a, Complex b) //D - Function that compares two complex numbers
{
    float magnitudeA, magnitudeB;
    magnitudeA = Magnitude(a);
    magnitudeB = Magnitude(b);
    if (magnitudeA == magnitudeB)
        return 0; // function returns 0 if both are equal
    else if (magnitudeA > magnitudeB)
        return 1; // function returns 1 if the first is greater than the second in magnitude
    else
        return -1; // function returns -1 if the second is greater than the first in magnitude
}
int main()
{
    int i, comp1, comp2;
    Complex num[3], sum, total;
    printf("---------------------------------------\n");
    printf("Enter complex number in the form (a+bi)\n");
    printf("---------------------------------------\n");
    for (i = 0; i < 3; i++) // for loop that collects values for all complex numbers
    {
        printf("Enter complex number #%d:\n", i+1);
        num[i] = read_Complex();
        printf("\n");
    }
    printf("---------------------------------------\n");
    for (i = 0; i < 3; i++) // for loop that prints all complex numbers input from the user
    {
        printf("%d)", i+1);
        print_Complex(num[i]);
    }
    printf("---------------------------------------\n");
    sum = add_Complex(num[0], num[1]);
    total = add_Complex(sum, num[2]);
    if (total.img >= 0)
        printf("The Sum of Complex Numbers is %.2f + %.2fi\n",total.real, total.img);
    else
        printf("The Sum of Complex Numbers is %.2f - %.2fi\n",total.real, fabs(total.img));

    comp1 = Compare_Complex(num[0], num[1]);
    switch (comp1)
    {
    case 0: // case in which the first two complex numbers are equal
        comp2 = Compare_Complex(num[0], num[2]);
        switch (comp2) // nested switch compares third complex number with the greatest one from the first two
        {
        case 0:
            printf("All of the complex numbers are equal in magnitude.\n");
            for (i = 0; i < 3; i++)
                print_Complex(num[i]);
            break;
        case 1:
            printf("The first two complex numbers are equal in magnitude and greater than the third.\n");
            print_Complex(num[0]);
            print_Complex(num[1]);
            break;
        case -1:
            printf("The third is the largest.\n");
            print_Complex(num[2]);
            break;
        }
        break;
    case 1: // case in which the first complex is greater than the second
        comp2 = Compare_Complex(num[0], num[2]);
        switch (comp2)
        {
        case 0:
            printf("The first and third complex numbers have equal magnitude that is higher than that of the second complex number.\n");
            print_Complex(num[0]);
            print_Complex(num[2]);
            break;
        case 1:
            printf("The first complex number has the highest magnitude.\n");
            print_Complex(num[0]);
            break;
        case -1:
            printf("The third complex number has the highest magnitude.\n");
            print_Complex(num[2]);
            break;
        }
        break;
    case -1: // case in which the second complex number is greater than the first
        comp2 = Compare_Complex(num[1], num[2]);
        switch (comp2)
        {
        case 0:
            printf("The second and third complex numbers have equal magnitude that is higher than that of the first complex number.\n");
            print_Complex(num[1]);
            print_Complex(num[2]);
            break;
        case 1:
            printf("The second complex number has the highest magnitude.\n");
            print_Complex(num[1]);
            break;
        case -1:
            printf("The third complex number has the highest magnitude.\n");
            print_Complex(num[2]);
            break;
        }
        break;
    }
    printf("---------------------------------------\n"); 
    return 0;
}
