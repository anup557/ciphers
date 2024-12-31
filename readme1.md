
## State Representation
Suppose $s_{1279} s_{1278} \cdots s_0$ is one dimensional representation of the state.

![twinkle state](state.pdf)

## Author's Representation of the State (Slice wise)
The author's stored the state in s[80] where $s[80] = {slice_0, slice_1, \cdots, slice_{79}}$,
with

$slice_0 = s_{15} \cdots s_{0}$
$slice_1 = s_{31} \cdots s_{16}$
$\vdots$
$slice_{79} = s_{1279} \cdots s_{1264}$

## Ours Representation of the State (Lane wise)
Our representation of a twinkle state is similar with the authors, though storing process of the
state into the variables are different. We use two 64 bit variable to store each lane. Hence
`uint64_t s[32]` is needed to store a full state into the variable `s`.

$0-th lane = s[0]||s[1] = s_{16\times 79} \cdots s_{16\times 64} ||  s_{16\times 63} \cdots s_{16\times 0}$ 
