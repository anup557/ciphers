// /* gist: code for klein-64 cipher in rust. The cipher is given in "https://www.researchgate.net/-
// -publication/221622631_KLEIN_A_new_family_of_lightweight_block_ciphers". The oracle has passed */
// /* test vectors given in the paper. */
// /* ------------------------------------------------------------------------------------------ */

mod oracle;

// defining number of rounds for klein cipher
const NO_OF_ROUNDS: u8 = 12;

fn main(){
    // taking the msg and key
    let mut msg: u64 = 0xffff_ffff_ffff_ffff_u64; 
    let key: u64 = 0x1234_5678_90ab_cdef_u64; 

    // apply oracle
    msg = oracle::oracle(msg, key);

    println!("\ncip: {:x}", msg);
}
