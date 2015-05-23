var fs = require('fs');  
var PI = 3.15;

function writeSin()
{
	var file = "sin.h";  
	
	var str = "const float _sinTable[] = { ";
	
	for(var i=0;i<= 2 * PI * 10000;i++)
	{
		if(i%10 == 0)
			str = str+"\n";
		
		if(i<2 * PI * 10000)
			str = str + Math.sin(i/10000).toPrecision(16) + "\t,";
		else
			str = str + Math.sin(i/10000).toPrecision(16) + "};";
	}
	
	fs.writeFile(file, str, function(err){  
        if(err)  
            console.log("fail " + err);  
        else  
            console.log("ok");  
    });  
}

function writeCos()
{
	var file = "cos.h";  
	
	var str = "const float _cosTable[] = { ";
	
	for(var i=0;i<= 2 * PI * 10000;i++)
	{
		if(i%10 == 0)
			str = str+"\n";
		
		if(i<2 * PI * 10000)
			str = str + Math.cos(i/10000).toPrecision(16) + "\t,";
		else
			str = str + Math.cos(i/10000).toPrecision(16) + "};";
	}
	
	fs.writeFile(file, str, function(err){  
        if(err)  
            console.log("fail " + err);  
        else  
            console.log("ok");  
    });  
}

function writeTan()
{
	var file = "tan.h";  
	
	var str = "const float _tanTable[] = { ";
	
	for(var i=0;i<= 2 * PI * 10000;i++)
	{
		if(i%10 == 0)
			str = str+"\n";
		
		if(i<2 * PI * 10000)
			str = str + Math.tan(i/10000).toPrecision(16) + "\t,";
		else
			str = str + Math.tan(i/10000).toPrecision(16) + "};";
	}
	
	fs.writeFile(file, str, function(err){  
        if(err)  
            console.log("fail " + err);  
        else  
            console.log("ok");  
    });  
}

function writeASin()
{
	var file = "asin.h";  
	
	var str = "const float _asinTable[] = { ";
	
	for(var i=0;i<= 10000;i++)
	{
		if(i%10 == 0)
			str = str+"\n";
		
		if(i<10000)
			str = str + Math.asin(i/10000).toPrecision(16) + "\t,";
		else
			str = str + Math.asin(i/10000).toPrecision(16) + "};";
	}
	
	fs.writeFile(file, str, function(err){  
        if(err)  
            console.log("fail " + err);  
        else  
            console.log("ok");  
    }); 
}

function writeACos()
{
	var file = "acos.h";  
	
	var str = "const float _acosTable[] = { ";
	
	for(var i=0;i<= 10000;i++)
	{
		if(i%10 == 0)
			str = str+"\n";
		
		if(i<10000)
			str = str + Math.acos(i/10000).toPrecision(16) + "\t,";
		else
			str = str + Math.acos(i/10000).toPrecision(16) + "};";
	}
	
	fs.writeFile(file, str, function(err){  
        if(err)  
            console.log("fail " + err);  
        else  
            console.log("ok");  
    }); 
}

function writeATan()
{
	var file = "atan.h";  
	
	var str = "const float _atanTable[] = { ";
	
	for(var i=0;i<= 2 * PI * 10000;i++)
	{
		if(i%10 == 0)
			str = str+"\n";
		
		if(i<2 * PI * 10000)
			str = str + Math.atan(i/10000).toPrecision(16) + "\t,";
		else
			str = str + Math.atan(i/10000).toPrecision(16) + "};";
	}
	
	fs.writeFile(file, str, function(err){  
        if(err)  
            console.log("fail " + err);  
        else  
            console.log("ok");  
    }); 
}


writeSin();
writeCos();
writeTan();
writeASin();
writeACos();
writeATan();