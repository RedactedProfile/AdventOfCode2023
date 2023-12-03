Day 1 is the warm up day. 
It's the easy day.
It's what gets you interested in the concept of AOC.
It's what gets you familiar with the mechanics of AOC.
It's what whets your appetite for more AOC. 

And yet. I found the first day an awkward challenge.   

Getting past the first part was mostly fine. But I took a while getting to it. 
I had this interesting idea I wanted to approach where I'd have this main "Explorer" front-end CLI "text adventure" type experience 
and each of the days would be a DLL file that adhered to a rigidly defined API that the explorer could then execute. And each DLL would link to a
static "commonlib" for shared code between them. 

Nothing made me realize that I have no idea what I'm doing with making and using Libraries than that experience, I spent _hours_ tinkering with
this and nothing would work, heck it was past the afternoon and I hadn't even started reading the first puzzle! And then I realized too.. that even
if I _did_ get this working.. I have no idea how to test the software, I know nothing about lib development or how to work with it. 

This is why at this state, the project structure looks the way it does. 

I abandoned the whole DLL thing and just decided to continue what I did last year, in that each day is it's own binary executable as an islanded subproject in the larger solution. 
it was easier this way. 

And then, came the problem. 

Oh, I saw the solution in my head... in other languages.  
But I'm using C. And in C, there's no such thing as python's `rfind()`.

Nonetheless knocking out the first part's solution was fairly simple. Had some boilerplate to write first, outputting the header, the common opening of the input file and reading the contents, that kind of thing, and it took me minute, I'm fairly new to C so this was all an experience. 

I tried something else new this time around. I decided to enlist the help of ChatGPT. 
Now, wait, before you bring out the pitchforks, let me explain _what_ I used of ChatGPT and how much of it I used in the code: _nothing_.
For, you see, i'm not using ChatGPT to generate my solutions for me, but I discovered pretty early in the day that ChatGPT knows an awful lot about C and how it works.. so I used it as an ever-present 24/7 accessable peer-coder that I can ask a million questions of and ask for clarifications on concepts and ask for alternative suggestions of, and it _wont_ roll it's eyes at me nor will it leave mid-way through an explanataion. 

Some real questions I asked of it:  
"In C, how do I initialize a char array later?"   
"in C how to pass an element of an array as a reference to a function"  

and my favorite was this  
"in C how to remove a matching string from inside another string"  
It provided a detailed explanation and a small code example of how it should work, but I wasn't satisfied with the answer  
"what to do if found+len is 0 though?"  
This was the part that made me smack my face in amazement. 

> If found + len evaluates to 0 (i.e., if found points to the start of the string), directly using memmove in this situation would result in undefined behavior. It's crucial to ensure that the pointer arithmetic doesn't lead to a null or invalid memory access.
> 
> One way to handle this scenario is by checking if found + len points to the null terminator, which indicates the end of the string. In this case, there's nothing to move, and it's safe to proceed. You can do this by checking if *(found + len) (the character at the position after the found substring) is the null terminator '\0'.
> 
> Here's an updated version of removeSubstring() that includes this check:

Look at that language "Here's an updated version of"- CRAZY. But I still wasn't satisfied
"but then how do I remove the characters found at the start of the string?"  
And wouldn't you know it? It updated it _again_ with the context of the last two results to include this edge case.  

This isn't a function I would use, but it did demonstrate to me, clearly, how to get something done.  This was incredible. 

Part 1, Done. 

Part 2 though?  What a nightmare.