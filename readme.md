Reversible Linear Congruential Generator
========================================

A header-only implementation of a pseudorandom number generator 
which supports generating random numbers in two directions.

Modification (AB): allow arbitrary period to be used (when other parameters chosen correctly).

What do you mean reversible?
----------------------------

Most random number generators support only generation of numbers 
in one direction:

```c++
Rng myRng(seed)
int rnd1 = myRng.next();
int rnd2 = myRng.next();
int rnd3 = myRng.next();
```

With this implementation you can do:

```c++
rlcg::Reversible myRng(seed);
int rnd1 = myRng.next();
int rnd2 = myRng.next();
int rnd3 = myRng.prev(); // rnd3 == rnd1
int rnd4 = myRng.next(); // rnd4 == rnd2
```

Why would I want this?
----------------------

Sometimes it is useful to backtrace from a current random number to the 
previous ones without storing them along the way. This implementation 
stores only a 64-bit state internally, so the memory footprint is 
extremely small.


Are there other alternatives?
-----------------------------

Yes, these are the ones I'm aware of:


### Hashing

You could get the same functionality by using a hash 
function and an index.

```c++
class rrng {
    int index;
public:
    rrnd(int seed):index(seed){}
    int next(){ return hash(index++); }
    int prev(){ return hash(index--); }
};
```

where `hash(int)` is your favorite hash function


### Encryption

You could use a simple symmetric encryption algorithm to encrypt 
the previous number. I have not tested this, but in theory it should 
work.

How random are the generated numbers?
-------------------------------------

Basically the quality is the same as the as for a regular linear 
congruential generator, and it should be fine in most use cases. 
With the standard parameters, the generator has a period of 2^63.

See https://en.wikipedia.org/wiki/Linear_congruential_generator 
for more details.

You should be aware that (seed >> 32) is going to be one of the random
numbers. This would equal 0 if you seed with `time(0)`. Take care when
seeding if you are going to get back to the initial state.


License
-------

All code is released under the permissive zlib license

```
RLCG - Reversible Linear Congruential Generator
Copyright (c) 2013 Johan Klokkhammer Helsing

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.

Johan Klokkhammer Helsing
(johanhelsing@gmail.com)
```

Author
------

Johan Klokkhammer Helsing (johanhelsing@gmail.com)


Thanks
------

Jakob Progsch for his initial implementation.
