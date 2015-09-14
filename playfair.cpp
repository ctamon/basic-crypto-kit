// playfair.cpp
// author: C. Tamon (1.23.2009)
// to compile: g++ -g -o playfair playfair.cpp
// to run: playfair <key> <plaintext_file> 
// key is a string of lowercase letters
// creates 3 aux files: 
//	"tmp.txt" (plain text stripped of all non-lowercase letters)
//	"enc.txt" (containing encrypted text) 
//	"dec.txt" (containing decrypted text)

#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;

void listPrint( list<int>& );

int main( int argc, char *argv[] )
{
	char *inputFileName;
	char *playfairKey;
	int keyLength;

	// read arguments
	if ( argc < 3 ) {
		fprintf( stderr, "Usage: ./playfair <plaintext_file> <key>" );
		exit(1);
	}

	inputFileName = strdup( argv[1] );
	playfairKey = strdup( argv[2] );
	keyLength = strlen( playfairKey );

	// building the Playfair table
	list<int> pfl;
	for (int i = 0; i < 26; i++)
		pfl.push_back(i);
	//listPrint( pfl );

	for (int i = keyLength-1; i >= 0; i--) {
		pfl.remove(playfairKey[i]-'a');
		pfl.push_front(playfairKey[i]-'a');
	}
	//listPrint( pfl );

	// remove character 'j' (identified as 'i')
	pfl.remove('j'-'a');

	// transfer list to vector
	list<int>::iterator it;
	vector<int> pft;
	//cout << "PFT=";
	for (it = pfl.begin(); it != pfl.end(); it++) {
		pft.push_back(*it);
		//cout << "[" << *it << "]";
	}
	//cout << endl;

	// Showing the 5x5 Playfair table
	// Also remember (row,col) position of each letter
	int row[26], col[26];

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			row[pft[5*i+j]] = i;
			col[pft[5*i+j]] = j;
			printf( "%3c", 'a' + pft[5*i+j] );
		}
		cout << endl;
	}
	row['j'-'a'] = row['i'-'a'];
	col['j'-'a'] = col['i'-'a'];
	
#ifdef DEBUG
	/* show row-column of table */
	for (int i = 0; i < 26; i++) {
        fprintf( stderr, "[%c:%d;%d]", 'a'+i, row[i], col[i] );
    }
    cout << endl;
#endif

	// testing encryption
	FILE *inputFile, *outputFile;
	int inputChar1, inputChar2;
	int outputChar1, outputChar2;
	int row1, col1;
	int row2, col2;

    // strip all but lower-case letters
    FILE *tmpFile;
    int tmpChar;
    
	inputFile = fopen( inputFileName, "r" );
	tmpFile = fopen( "tmp.txt", "w" );
	while ( (tmpChar = fgetc( inputFile )) != EOF ) {
            if ( !islower(tmpChar) ) continue;
            fputc( tmpChar, tmpFile );
    }
    fclose( inputFile );
    fclose( tmpFile );

    // encrypting	
	//int lineCount = 0;
	inputFile = fopen( "tmp.txt", "r" );
	outputFile = fopen( "enc.txt", "w" );	
	while ( (inputChar1 = fgetc( inputFile )) != EOF ) {
			// check if second character exists
			if ( (inputChar2 = fgetc( inputFile )) == EOF ) {
				fputc( inputChar1, outputFile );
				break;
			}
			else if ( inputChar2 == inputChar1 ) {
                ungetc( inputChar2, inputFile );
                inputChar2 = 'x';
            }
			
			// encrypt two consecutive characters
			row1 = row[inputChar1-'a']; 
            col1 = col[inputChar1-'a'];
			row2 = row[inputChar2-'a']; 
            col2 = col[inputChar2-'a'];
                        
            if ( row1 == row2 ) {
                outputChar1 = 'a'+pft[5*row1+((col1+1)%5)];
                outputChar2 = 'a'+pft[5*row1+((col2+1)%5)];
            }
            else if ( col1 == col2 ) {
                outputChar1 = 'a'+pft[5*((row1+1)%5)+col1];
                outputChar2 = 'a'+pft[5*((row2+1)%5)+col1];
            }
            else {
                outputChar1 = 'a'+pft[5*row1+col2];
                outputChar2 = 'a'+pft[5*row2+col1];     
            }

            fputc( outputChar1, outputFile );
            fputc( outputChar2, outputFile );

			/*lineCount++;
			if ( lineCount == 10 ) {
				fprintf( outputFile, "\n" );
				lineCount = 0;
			}*/
	}
	fclose( inputFile );
	fclose( outputFile );

    // testing decryption
	inputFile = fopen( "enc.txt", "r" );
	outputFile = fopen( "dec.txt", "w" );	
	while ( (inputChar1 = fgetc( inputFile )) != EOF ) {
			// check if second character exists
			if ( (inputChar2 = fgetc( inputFile )) == EOF ) {
				fputc( inputChar1, outputFile );
				break;
			}
			else if ( inputChar2 == inputChar1 ) {
                ungetc( inputChar2, inputFile );
                inputChar2 = 'x';
            }
			
			// decrypt two consecutive characters
			row1 = row[inputChar1-'a']; 
            col1 = col[inputChar1-'a'];
			row2 = row[inputChar2-'a']; 
            col2 = col[inputChar2-'a'];
                        
            if ( row1 == row2 ) {
                outputChar1 = 'a'+pft[5*row1+((col1+4)%5)];
                outputChar2 = 'a'+pft[5*row1+((col2+4)%5)];
            }
            else if ( col1 == col2 ) {
                outputChar1 = 'a'+pft[5*((row1+4)%5)+col1];
                outputChar2 = 'a'+pft[5*((row2+4)%5)+col1];
            }
            else {
                outputChar1 = 'a'+pft[5*row1+col2];
                outputChar2 = 'a'+pft[5*row2+col1];     
            }

            fputc( outputChar1, outputFile );
            fputc( outputChar2, outputFile );
	}
	fclose( inputFile );
	fclose( outputFile );
    
	getchar();    	
}

void listPrint( list<int>& t )
{
	list<int>::iterator i;
	for (i = t.begin(); i != t.end(); i++)
		cout << "[" << *i << "]";
	cout << endl;

}


