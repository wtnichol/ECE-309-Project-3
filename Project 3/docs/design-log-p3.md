# Project 3 Design Log
**Author:** <Will Nichols> · **P2 code used:** <instructor reference>
**Tools used:** <I used Codex to help with finding issues with my code and to check if the code compiled. It also helped guide me through a problem when I got stuck, generally by giving me code snippet examples to help me figure out the problem. It also helped me find functions in the allowed standard libraries to use, since I am not familiar with them.>

<!-- 500–800 words. An argument, not a diary: say what you decided, what
     you rejected, and what evidence backs it up. Delete these comments
     before submitting. -->

## 1. Hash table
     With the hash table I chose to use a vector of buckets using the chaining method, each containing a pointer to a tool. The reason for this was that when hashing the name and when a collision was detected it was easier and faster to drop it at the end of the bucket using the push back function rather than to loop through and find an empty bucket with linear probing.
     I used the string hash supplied in the spec to create a key for the hash table, since I was unfamiliar with how to create a key.
     I started with 8 buckets within the hash table as I know our simulated model only used a small number of tools, it would save memory when the program executes.
     I chose a load threshold of 1.0 meaning that when there are as many tools as there are buckets, the table will grow. I chose to have it grow prior to accepting another tool, despite using chaining. This reason was primarily so the links in the chain do not get overfilled, and make it harder and time consuming to search for a tool.
     I decided to double the bucket count when I grew the table because after a few grows it'll be harder to fill up. I then recalculated the hash of every existing tool and moved that to the resized hash. 
     Typically when adding tools to the hash its cheap because it will only take one run through to add it, but resizes are expensive. Fortunately as the hash resizes the time between those resizes grows exponentially meaning that after each resize it'll take longer and longer for another resize to happen. This allows for the hash table to reach a Big O of O(1);

## 2. Pointer stability
     The reason why a tool returned by find() survives a resize is that ech tool is stored separately in memory as a unique pointer. Each bucket and chain link doesn't own the tool, it only contains the pointer to where in memory that tool is stored. What happens is that the resize doesn't destroy that unique pointer. The resize creates a new set of buckets that is double the size of the previous and then recalculates the hash and transfers ownership of those pointers to the new bucket. Transferring ownership doesn't change where in memory that pointer is stored so the tools are unaffected by the resizing of the hash table.
## 3. Calculator trace
     When tracking the top of the stack is the right side of the vector.
     It starts off by pushing 2 onto the top of the values stack [2], followed by the multiplication operator on the ops stack [*]. Then you push the left parenthesis onto the top of the ops stack [*, (]. From then 3 is pushed onto the vals stack [2, 3]. Then + is pushed to the top of the ops stack [*, (, +] and then 4 is pushed onto the vals stack [2, 3, 4]. Right parenthesis is not pushed. Calculate the parts inside the parenthesis: pop 4 and 3 into two tmp variables, and pop + into another tmp variable. Pop the left parenthesis. I then calculate 4 + 3 and push the result: 7 onto the vals stack [2, 7] and [*]. I then try to push the next op, - onto the stack but the * is a higher priority on the stack so you calculate something. You pop 2 and 7 off the stack [] and * off the stack [] and you do 2 * 7 = 14. You then push 14 onto the vals stack [14] and then push - on the stack [-]. The last token is a 5 which is pushed onto the stack [14, 5]. You then pop 14 and 5 and - with the stacks being [] and []. You perform the calculation 14 - 5 = 9 and then put *out = 9.

## 4. Errors as values
<!-- Why do tools, resolve() and calc_evaluate() return errors instead of
     throwing? When IS an exception the right choice? -->
     Problems that stem from these tools don't always mean there is a bug. Such as dividing by zero, thats physically not possible in math, not a problem due to the programming. What these errors do is show that the tool failed due to issues extending beyond the programs ability to solve, menaing that it can just report that there it failed to accomplish something and why it failed instead of just trying to get around the core issue. Exceptions are typically only appropriate when when failures are caused by programming mistakes rather than just bad requests.


## 5. What I would design differently
     When I was working on the calculator eval I noticed I pasted the part where I pop two vals plus the operation, do the calculation, and then push the result onto the values stack 3 separate times. I realize now that I could've made that a separate function and called that 3 times saving around 64 lines of code.