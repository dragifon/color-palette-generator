#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

using namespace std;

enum hueModel {SRGB, CIE, CIE2,CIE3};
enum svModel {LINEAR, SQRT2, SQRTN};
enum sortOrder {VSH, VHS, SVH, SHV, HVS, HSV};

int hsvtorgb(int hue, int sat, int val)
{
	cout << "Old HSV" << hue << "," << sat << "," << val << " ";
    //Set HSV
    hue %= 360; //set hue to less than 360
    if (hue < 0) //and more than zero
    {
    	hue += 360;
    }
    
    if (sat > 100) //set sat to max of 100
    {
        sat = 100;
    }
    else if (sat < 0) //set sat to min of 0
    {
        sat = 0;
    }
    
    if (val > 100) //set val to max of 100
    {
        val = 100;
    }
    else if (val < 0) //set val to min of 0
    {
        val = 0;
    }
    cout << "New HSV" << hue << "," << sat << "," << val << " ";
    
    //Get RGB
    int r, g, b;
    int cap = 255;
    
    if (val == 0) //makes black
    {
        r = 0;
        g = 0;
        b = 0;
        cout << " black ";
    }
    else if (sat == 0 && val > 0) //makes grays
    {
        r = cap * val / 100;
        g = cap * val / 100;
        b = cap * val / 100;
        cout << " grays ";
    }
    else if (val > 0 && sat > 0) //makes hues
    {
        if (hue >= 0 && hue <= 60) //if hue between red and yellow
        {
            r = cap * val / 100; //sets red as val
            b = cap * val * (100 - sat) / 10000; //sets blue as sat
            g = ((r - b) * hue / 60) + b; //sets green as hue ratio times val and sat range plus sat
            cout << " oranges ";
        }
        else if (hue >= 60 && hue <= 120) //if hue between yellow and green
        {
            g = cap * val / 100; //sets green as val
            b = cap * val * (100 - sat) / 10000; //sets blue as sat
            r = ((g - b) * (120 - hue) / 60) + b; //sets red as hue ratio times val and sat range plus sat
            cout << " limes ";
        }
        else if (hue >= 120 && hue <= 180) //if hue between green and cyan
        {
            g = cap * val / 100; //sets green as val
            r = cap * val * (100 - sat) / 10000; //sets red as sat
            b = ((g - r) * (hue - 120) / 60) + r; //sets blue as hue ratio times val and sat range plus sat
            cout << " turquoises ";
        }
        else if (hue >= 180 && hue <= 240) //if hue between cyan and blue
        {
            b = cap * val / 100; //sets blue as val
            r = cap * val * (100 - sat) / 10000; //sets red as sat
            g = ((b - r) * (240 - hue) / 60) + r; //sets green as hue ratio times val and sat range plus sat
            cout << " blues ";
        }
        else if (hue >= 240 && hue <= 300) //if hue between blue and magenta
        {
            b = cap * val / 100; //sets blue as val
            g = cap * val * (100 - sat) / 10000; //sets green as sat
            r = ((b - g) * (hue - 240) / 60) + g; //sets red as hue ratio times val and sat range plus sat
            cout << " violets ";
        }
        else if (hue >= 300 && hue <= 360) //if hue between magenta and red
        {
            r = cap * val / 100; //sets red as val
            g = cap * val * (100 - sat) / 10000; //sets green as sat
            /*HERE*/b = ((r - g) * (360 - hue) / 60) + g; //sets blue as hue ratio times val and sat range plus sat
            cout << " pinks ";
        }
        else
        {
            cout << " ERROR in hsvtorgb() unrecognized hue ";
        }
    }
    else
    {
        cout << "ERROR in hsvtorgb() unrecognized conditions";
    }
    
    cout << " Old RGB " << r << "," << g << "," << b << " ";
    cout << " New RGB " << r * 256 * 256 + g * 256 + b << endl;
    return r * 256 * 256 + g * 256 + b;
}

int hue(int n, int hues, hueModel model, int hueOffset = 0) //allows easy switching of hue calculation formula
{
	n = 360 * n / hues; //linear hue formula
	n = (n + hueOffset)%360;
	if (n < 0)
	{
		n += 360;
	}
	
	float r1, y1, g1, c1, b1, m1;
	
    switch (model)
    {
        case CIE: //more accurate hue formula
            if (n >= 60 && n <= 90)
            {
                n += n - 60;
            }
            else if (n >= 90 && n <= 180)
            {
            	n += (180-n)/3;
            }
            else if (n >= 240 && n <= 270)
            {
            	n += n - 240;
            }
            else if (n >= 270 && n <= 360)
            {
            	n += (360-n)/3;
            }
            break;
        case CIE2: //full spectrum from red to violet
            r1 = 0;
            y1 = 58.72;
            g1 = 92.56;
            c1 = 180;
            b1 = 238.79;
            m1 = 272.56;
            if (n >= r1 && n <= y1)
            {
            	n = ((n - r1) / (y1 - r1)) * 60 + 0;
            }
            else if (n >= y1 && n <= g1)
            {
                n = (n - y1) / (g1 - y1) * 60 + 60;
            }
            else if (n >= g1 && n <= c1)
            {
            	n = ((n - g1) / (c1 - g1)) * 60 + 120;
            }
            else if (n >= c1 && n <= b1)
            {
            	n = ((n - c1) / (b1 - c1)) * 60 + 180;
            }
            else if (n >= b1 && n <= m1)
            {
            	n = ((n - b1) / (m1 - b1)) * 60 + 240;
            }
            else if (n >= m1 && n <= 360)
            {
            	n = ((n - m1) / (360 - m1)) * 60 + 300;
            }
            break;
//TODO HERE BROSIVE
        case CIE3: //full spectrum from red to violet
            r1 = 351.2;
            y1 = 66.74;
            c1 = 171.2;
            b1 = 246.74;
            
            if (n >= r1)
            {
            	n -= 360;
            }
            
            if (n >= r1-360 && n <= y1)
            {
        		r1 -= 360;
            	n = ((n - r1) / (y1 - r1)) * 60 + 0;
            }
            else if (n >= y1 && n <= c1)
            {
                n = (n - y1) / (c1 - y1) * 60 + 60;
            }
            else if (n >= c1 && n <= b1)
            {
            	n = ((n - c1) / (b1 - c1)) * 60 + 180;
            }
            else if (n >= b1 && n <= r1)
            {
            	n = ((n - b1) / (r1 - b1)) * 60 + 240;
            }
            break;
      	case SRGB:
        default: break;
    }
    
    return n;
}
    
int sv(int n, int m, svModel model) //allows easy switching of saturation calculation formula
{
    switch (model)
    {
        case SQRT2: //sqrt(2) based value formula:
            if (m <= 4)
        	{
        		n = (m - n) * 4;
        	}
        	else if (m <= 7)
        	{
        		n = (m - n) * 2;
        	}
        	else
        	{
        		n = m - n;
        	}
            n = 100 * pow(8, (-n / 12.0));
            cout << "s" << n << endl;
            break;
        case SQRTN: //sqrt(n) based value formula (gets to true 0 and 1)
        	
        	n = 100 * pow(2,(n / float(m))) - 100;
        	break;
        case LINEAR: 
        default:
        	n = n * 100 / m; //linear saturation formula
    }
    
    return n;
}

string colorChannel(int n)
{
	string line = "";
	
    if (n < 100) //puts leading spaces in front of n values under 100
    {
        line += " ";
        if (n < 10) //puts leading spaces in front of n values under 10
        {
            line += " ";
        }
    }
    
    line += to_string(n) + " "; //writes n value
    
    return line;
}

string writeRGB(int rgb) //Write rgb to line
{
    int r = (rgb-(rgb%(256*256)))/(256*256);
    int g = (rgb%(256*256)-((rgb%(256*256))%256))/256;
    int b = rgb%256;
    string line = "";
    
	line += colorChannel(r);
	line += colorChannel(g);
	line += colorChannel(b);
	
    //write rgb names
    line += "rgb" + to_string(r) + "," + to_string(g) + "," + to_string(b);
    
    return line;
}

string sort(int hues, int sats, int vals, sortOrder o = VSH, hueModel hm = SRGB, svModel sm = LINEAR, svModel vm = LINEAR, int ho = 0) //allows easy switching of the sort method
{
    int h, s, v, x, y, z;
	string output = "";
	
	switch (o)
    {
    	case HSV:
    		z = hues;
    		y = sats;
    		x = vals;
    		break;
		case HVS:
    		z = hues;
    		y = vals;
    		x = sats;
			break;
		case SHV:
    		z = sats;
    		y = hues;
    		x = vals;
			break;
		case SVH:
    		z = sats;
    		y = vals;
    		x = hues;
			break;
		case VHS:
    		z = vals;
    		y = hues;
    		x = sats;
			break;
    	case VSH:
		default: 
    		z = vals;
    		y = sats;
    		x = hues;
    }
    for (int i = x; i > 0; i--)
    {
        for (int j = y; j > 0; j--)
        {
            for (int k = z; k > 0; k--)
            {
                
                switch (o)
                {
                	case HSV:
                		h = k;
                		s = j;
                		v = i;
                		break;
					case HVS:
						h = k;
                		v = j;
                		s = i;
						break;
					case SHV:
						s = k;
                		h = j;
                		v = i;
						break;
					case SVH:
						s = k;
                		v = j;
                		h = i;
						break;
					case VHS:
						v = k;
                		h = j;
                		s = i;
						break;
                	case VSH:
            		default: 
                		v = k;
                		s = j;
                		h = i;
                }
                h = hue(hues-h,hues,hm,ho);
                s = sv(s,sats,sm);
                v = sv(v,vals,vm);
                output += writeRGB(hsvtorgb(h,s,v));
                output += " hsv" + to_string(h) + "," + to_string(s) + "," + to_string(v) + "\n";
            }
        }
    }
    
    for (int v = vals-1; v >= 0; v--) //No sat
    {
        output += writeRGB(hsvtorgb(0, 0, sv(v, vals, vm)));
        output += " hsv" + to_string(0) + "," + to_string(0) + "," + to_string(sv(v, vals, vm)) + "\n";
    }
    output += writeRGB(hsvtorgb(0, 0, 0)); //black
    output += " hsv" + to_string(0) + "," + to_string(0) + "," + to_string(0) + "\n";
    
    output += writeRGB(hsvtorgb(0, 0, 100)); //white
    output += " hsv" + to_string(0) + "," + to_string(0)+ "," + to_string(100) + "\n";
    
    return output;
}

void createFile(int hues = 6, int sats = 3, int vals = 3, sortOrder so = VSH, hueModel hm = SRGB, svModel sm = LINEAR, svModel vm = LINEAR, int ho = 0) //creates a file for storing a palette AND writes a palette into it
{
    int filenumber = 1;
    int columns = hues;
    string filename = "";
    string location = "/home/lapifon/.config/GIMP/2.10/palettes/";
    string extension = ".gpl";
    
    switch(hm) //add hue model to filename
    {
		case CIE:
			filename += "cieA";
			break;
		case CIE2:
			filename += "cieB";
			break;
		case CIE3:
			filename += "cieC";
			break;
    	case SRGB:
    	default:
    		filename += "srgb";
    }
    
    filename += " " + to_string(hues) + " " + to_string(sats) + " " + to_string(vals) + " " + to_string(ho) + "o "; // add hues sats and vals to filename
    
    switch(sm) //add sat model to filename
    {
    	case SQRT2:
    		filename += "S";
    		break;
		case SQRTN:
			filename += "N";
			break;
    	case LINEAR:
		default:
			filename += "L";
    }
    
    switch(vm) //add val model to filename
    {
    	case SQRT2:
    		filename += "S";
    		break;
		case SQRTN:
			filename += "N";
			break;
    	case LINEAR:
		default:
			filename += "L";
    }
    
    filename += " ";
    
    switch (so) //add sortOrder to filename
    {
    	case HSV:
    		filename += "hsv";
    		columns = hues * sats;
    		if (columns > 64)
    		{
    			columns /= sats;
    		}
    		break;
        case HVS:
            filename += "hvs";
            columns = hues * vals;
    		if (columns > 64)
    		{
    			columns /= vals;
    		}
            break;
        case SHV:
        	filename += "shv";
        	columns = sats * hues;
    		if (columns > 64)
    		{
    			columns /= hues;
    		}
        	break;
    	case SVH:
    		filename += "svh";
    		columns = sats * vals;
    		if (columns > 64)
    		{
    			columns /= vals;
    		}
			break;
		case VHS:
			filename += "vhs";
			columns = vals * hues;
    		if (columns > 64)
    		{
    			columns /= hues;
    		}
			break;
		case VSH:
        default:
        	filename += "vsh";
        	columns = vals * sats;
    		if (columns > 64)
    		{
    			columns /= sats;
    		}
    }
    
    for (bool isNew = false; isNew == false; filenumber++) //check for new files
    {
        ifstream file; //create input stream called file 
        file.open(location + filename + to_string(filenumber) + extension); //try opening file at location in parentheses
        
        if (file.is_open()) //if file opened it existed and isn't new so close it
        {
            file.close();
            cout << "File " << filename+to_string(filenumber)+extension << " at " << location << " exists" << endl;
        }
        else //if file not opened it is probably new so create it
        {
            ofstream file; //create output stream called file
            file.open(location+filename+to_string(filenumber)+extension); //create and/or open file at location in parentheses
            
            //FILE CONTENTS GO HERE
            file << "GIMP Palette\n";
            file << "Name: " << filename+to_string(filenumber) << endl;
            file << "Columns: " << columns << endl;
            file << "# Comments " << endl;
            
            file << sort(hues, sats, vals, so, hm, sm, vm, ho);
            
            file.close(); //close file
            
            cout << "Palette " << filename+to_string(filenumber)+extension << " created at " << location << endl;
            isNew = true; //new palette created so leave for loop
        }
    }
}
    
int main()
{
	int h = 16;
	int s = 4;
	int v = 4;
	hueModel hm = CIE2;
	int ho = 0;
    createFile(h,s,v,VSH,hm,SQRT2,SQRT2,ho);
    createFile(h,s,v,VHS,hm,SQRT2,SQRT2,ho);
    createFile(h,s,v,VSH,hm,SQRTN,SQRTN,ho);
    createFile(h,s,v,VHS,hm,SQRTN,SQRTN,ho);
    createFile(h,s,v,VSH,hm,LINEAR,SQRT2,ho);
    createFile(h,s,v,VHS,hm,LINEAR,SQRT2,ho);
    
    return 0;
}
//TODO
//	fadsff
// faas
/*TODO eat it
	husdds
	nano
-Set up sort method selection (hsv, vsh)
-Change hsv values written to file to be 0-360, 0-100, and 0-100 instead of hues, sats, and vals
*/
