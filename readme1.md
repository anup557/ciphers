
## State Representation
Suppose $s_{1279} s_{1278} \cdots s_0$ is one dimensional representation of the state, where each $s_i$, for $0 \leq i \leq 1279$ is one bit.

![twinkle state](state.pdf)

## Author's Representation of the State (Slice wise)
Author's stored the state in `uint32_t s[80]` where $s[80] = {slice_0, slice_1, \cdots, slice_{79}}$,
with

$$slice_0 = s_{15} \cdots s_{0}$$

$$slice_1 = s_{31} \cdots s_{16}$$

$$\vdots$$

$$slice_{79} = s_{1279} \cdots s_{1264}$$

## Ours Representation of the State (Lane wise)
Our representation of a twinkle state is similar with the authors, though storing process of the
state into the variables is different. We use two 64 bit variable to store each lane. Hence
`uint64_t s[32]` is needed to store a full state into the variable `s`.

$$
\begin{align*}
    0\text{-th lane} &= s[0]\parallel s[1] \\
    &= s_{16\times 79} \cdots s_{16\times 64} (\text{16 bit}) \parallel s_{16\times 63} \cdots s_{16\times 0} (\text{64 bit})
\end{align*}
$$

Similarly, for the $i$-th lane with $0 \leq i \leq 15$, 

$$s[2i]\parallel s[2i +1] = s_{\(16\times 79\) +i} \cdots s_{\(16\times 64\) +i} (\text{16 bit}) \parallel s_{\(16\times 63\) +i} \cdots s_{\(16\times 0\) +i} (\text{64 bit})$$

## Usage
To run the code, run the following command in linux terminal or run the main.c file.

```
gcc main.c -o out
./out
```

## Make a Twinkle State (Lane wise)
The following code insert the 80 bit number `0x11112222333344445555` into each lane of the state.

```
uint64_t *s = (uint64_t*)malloc(32*sizeof(uint64_t));                                           
for (int i=0; i<16; i++){                                                                       
    s[2*i] = 0x1111; s[2*i +1] = 0x2222333344445555;                                            
}                                                                                               
```

## Slice wise to Lane wise
Suppose $s_{1d}[80] = {slice_{79}, \cdots, slice_1, slice_0}$ stores one dimensional representation of
the twinkle state in slice wise fashion. The function `void _1d_to_lane_wise(uint64_t *s, uint16_t *s_1d)` stores the
state as the given figure in lane wise way into the variable `s[32]`.

## Advantage of Lane wise Implementation
- Each SBox, LR, Mixslice needs only 16 iterations (actually 32, as each lane stored into two 64 bit
  variable). While in slice wise fashion, they need 320, 1280 and 80 iterations respectively. 
