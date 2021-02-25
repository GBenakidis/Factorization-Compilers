#pragma once
#include <map>
#include <string>
using namespace std;

// dilwsh tou hash table
extern map<string, int> g_symbolTable;

// anakta tin timi mias metablitis apo ton pinaka simbolwn 
// (arxikopoihsh tis metablitis an den iparxoun akoma ston pinaka simbolown)
int GetInitValue(string name); 

// sinartisi pou thetei tin metabliti se kapoia timi pou tou dinoume
void SetValue(string name, int value);
