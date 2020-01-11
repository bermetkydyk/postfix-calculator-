#ifndef PostfixCalculator_H
#define PostfixCalculator_H

// includes
#include <string>
#include <stack>

using namespace std;

class PostfixCalculator
{

    public:
    
        // default constructor (does nothing)
        PostfixCalculator() {}
        
        // function to convert infix to postfix
        string convertToPostfix(string infix_expression);

        // function to calculate output given postfix
        double calculatePostfix(string postfix_expression);

        // function to test with an input file
        void testCalculator(string input_file_name);
        
    private:
        // Function to return precedence of operators 
        int prec(char c);
        
};

#endif
