#include "PostfixCalculator.hpp"
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include "PrecondViolatedExcep.hpp"
#include <fstream>

using namespace std;

string PostfixCalculator::convertToPostfix(string infix_expression)
{

    stack<char> astack;
    string postfixExp = "";
    char ch;


    for (int i = 0; i < infix_expression.length(); i++)
    {
        ch = infix_expression[i];

        switch (ch)
        {
            // all types of digits
            case '0': // append operand to end of postfix expression
          	case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
          	case '8': 
          	case '9':
            	// add the first character of the integer
            	postfixExp += ch;
            	// check if next character is a digit too
            	// ( 507 + 305 )
                //           index: 0 char: 5       output: 5
                // while: 1  index: 0 char: 0       output: 50
                // while: 2  index: 1 char: 7       output: 507
                // while: NA  index: 2 
                // while next character is a digit
            	while ( isdigit(infix_expression[i+1]) )
                {
                    // append next digit of integer
                	postfixExp += infix_expression[i+1];
                    i++;
                }
            
            	// put space after operand
            	postfixExp += " ";
            	
                break;
            case '(':
                astack.push(ch);
                break;
            case '+':
         	case '-':
          	case '*':
         	case '/':
            	// pop off the stack until we get to a closing parentheses or a operator with higher precedence
                while ( !astack.empty() && (astack.top() != '(') && (prec(ch) <= prec(astack.top())) ) 
                {
                  	// add it to expression all of the parts
                	postfixExp += astack.top();
                  	astack.pop();
                  
                  	postfixExp += " ";
                }
                
                astack.push(ch); // add the operator
                
            	break;
            case ')': // pop stack until matching '(' - step 4
            
                while( astack.top() != '(' )
                {
                    postfixExp += astack.top();
                    astack.pop();
                  
                  	postfixExp += " ";
                    
                    if (astack.empty())
                    {
                        // cout << "extra closing parentheses" << endl;
                        throw ( PrecondViolatedExcep("Unbalanced parenthesis") );
                        break;
                    }
                    

                }
                astack.pop();
                
                /*
                if (astack.empty())
                {
                    // throw an error
                    throw ( PrecondViolatedExcep("Unbalanced Parentheses problem!") );
                    
                }
                */
                
                break;
          	case ' ':
		case '\n':
		case '\r':
            	break; 
          	default: // if not a digit, space, parentheses, or basic operator
                // cout << "extra character" << endl;
            	throw ( PrecondViolatedExcep("Unknown operator") ); // throw an error
            	break;     

        }
    }
    
    // append to postfixExp the operators remaining in the stack - step 5
    while(!astack.empty())
    {
        // if any start parentheses were left on the stack
        if (astack.top() == '(')
        {
            // cout << "extra starting parentheses" << endl;
            throw ( PrecondViolatedExcep("Unbalanced parenthesis") );
        }
        
        postfixExp += astack.top();
        astack.pop();
      
      	postfixExp += " ";
    }
    
    return postfixExp;

}

// Function to return precedence of operators 
int PostfixCalculator::prec(char c)
{
    
    if (c == '*' || c == '/') 
    	return 2; 
    else if (c == '+' || c == '-') 
    	return 1; 
    else
    	return -1; 
}
  

double PostfixCalculator::calculatePostfix(string postfix_expression)
{
    
    stack<double> mystack;
  
  	char ch; 
  
    for(int i = 0; i < postfix_expression.length(); i++)
    {
   		ch = postfix_expression[i];
      
      	// if it is an operator
        if ( (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') )
        {
            double result = 0;
            double operand2 = mystack.top();
            mystack.pop();
            double operand1 = mystack.top();
            mystack.pop();
            
          	// check which operator we are using
            if (ch == '+')
            	result = operand1 + operand2; 
            else if (ch == '-')
              	result = operand1 - operand2; 
            else if (ch == '*')
              	result = operand1 * operand2; 
            else if (ch == '/')
              	result = operand1 / operand2; 
            
			// push the result on the stack
            mystack.push(result);
            

        }
      	else if ( isdigit(ch) )     // if it is a operand 
      	{ 
            // initialize empty string
            string numString = "";
            
            // add first character
            numString += ch;
            
            // while there are more digits
            while ( isdigit(postfix_expression[i+1]) )
            {
                // append next digit of integer
                numString += postfix_expression[i+1];
                i++;
            }
            
            double num = atof(numString.c_str());
            mystack.push(num);

        }
    }
  
    return mystack.top();

}

// convert it to postfix (don't forget to do this in a try-catch block, converttopostfix may throw an exception!)
// write the corresponding postfix expression into postfix.txt, but if converttopostfix throws and exception, write the error message instead
// evaluate the postfix expression if wellformed and write the result in results.txt, otherwise write the error message there too
void PostfixCalculator::testCalculator(string input_file_name)
{
	// input file stream and output file stream
    ifstream inFile;
  
    ofstream file1;
    file1.open("postfix.txt");
    
    ofstream file2;
    file2.open("results.txt");
    
    // inFile.open("input.txt");
    
  	inFile.open(input_file_name);
  
    if (inFile.fail()) 
    {
    	cout << "Error opening a file" << endl;
    	inFile.close();
    }
  	
 	 // go line by line through the file
    string line;
    while (getline(inFile, line))
    {
        try // try to convert to postfix
        {
        	//cout << convertToPostfix(line) << endl;
          	file1 << convertToPostfix(line) << endl;
           
          	//cout << calculatePostfix(convertToPostfix(line)) << endl;
            file2 << calculatePostfix(convertToPostfix(line)) << endl;
          
        }
        catch (PrecondViolatedExcep& e)
   		{
            //cout  << e.what() << endl;
            file1 << e.what() << endl;
            file2 << e.what() << endl;
    	}
       
    }
  
    inFile.close();
  	file1.close();
  	file2.close();
    //file.open("input.txt");


}
