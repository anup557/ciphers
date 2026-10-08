// gist: the oracle for klein-64 cipher in rust.
// ---------------------------------------------------------------------------------------------

// If the print details is true then only all the state values will print
// static PRINT_DETAILS : bool = true;
static PRINT_DETAILS : bool = false;

// using the number of rounds defining in the main file
use NO_OF_ROUNDS;

fn key_scheduling(key: u64) -> [u64; (NO_OF_ROUNDS as usize)+1]{
    // initializing round keys
    let mut round_key: [u64; (NO_OF_ROUNDS as usize)+1] = [0; (NO_OF_ROUNDS as usize)+1]; 

    for i in 0..NO_OF_ROUNDS+1{
        // the 0-th round key is the original master key
        if i == 0{
            round_key[i as usize] = key;
            continue;
        }

        // extracting a and b from previous round key
        let mut a: u32 = ((round_key[(i-1) as usize]>>32)&0xffffffff) as u32;
        let mut b: u32 = (round_key[(i-1) as usize]&0xffffffff) as u32;

        // shifting bytes in a and b
        a = (a<<8)|((a>>24)&0xff);
        b = (b<<8)|((b>>24)&0xff);

        // storing a in tmp for swapping
        let tmp: u32 = a;

        // modifying a by adding round number with it
        a = b^(((i as u32)&0xffff_ffff_u32) << 8);

        // xor-ing b with a (stored in tmp)
        b = tmp^b;

        // making new key just to apply sbox on the middle bytes of b
        let mut new_key: u64 = sbox((b&0x00_ff_ff_00) as u64);
        new_key = new_key&0x00_ff_ff_00;

        // making new b
        b = (b&0xff_00_00_ff) | ((new_key&0xff_ff_ff_ff) as u32);

        // making the new round key
        round_key[i as usize] = a as u64;
        round_key[i as usize] = (round_key[i as usize] << 32) | ((b as u64)&0xffff_ffff_ffff_ffff_u64);
    }

    /* for printing purpose */
    if PRINT_DETAILS == true{
        println!("\n********************************************************************************");
        println!("round keys:");
        println!("********************************************************************************");

        for i in 0..NO_OF_ROUNDS+1{
            println!("for round {}:\t {:x}", i, round_key[i as usize]);
        }
    }

    return round_key;
}


fn sbox(msg:u64) -> u64{
    /* sbox table of klein cipher */
    let sbox_table:[u8; 16] = [0x7, 0x4, 0xa, 0x9, 0x1, 0xf, 0xb, 0x0, 0xc, 0x3, 0x2, 0x6, 0x8, 0xe, 0xd, 0x5];

    let mut cip:u64 = 0;

    // applying sbox on each nibble
    for i in (0..16).rev(){
        let nibble:u64 = sbox_table[((msg >> 4*i)&0xf) as usize] as u64;
        cip = (cip << 4)|(nibble&0xf);
        }

    /* for printing purpose */
    if PRINT_DETAILS == true{
        println!("after sbox:\t\t {:x}", cip);
    }   

    return cip;
}


fn rot_nibble(msg:u64) -> u64{
    // here rotate nibble is just circular shifting of 16-bit within 64-bit
    let mut cip:u64 = (msg<<16)&0xffff_ffff_ffff_ffff_u64;
    cip = cip|((msg>>48)&0xffff_u64);

    /* for printing purpose */
    if PRINT_DETAILS == true{
        println!("after rot nibble:\t {:x}", cip);
    }   

    return cip;
}


fn mix_nibble(msg:u64) -> u64{
    // initializing cip
    let mut cip:u64 = 0;

    // as there are 8 bytes in the states on which mix nibble will apply 
    for half in 0..2{
        // initializing variables for multipication
        let mut a:[u64; 4] = [0; 4];
        let mut b:[u64; 4] = [0; 4];
        let mut r:[u64; 4] = [0; 4];
        let mut h:u64;

        // initialing bytes into r[]
        for i in 0..4{
            r[i] = (msg>>(56 - 32*half - 8*i))&0xff;
        }

        for i in 0..4{
            a[i] = r[i];
            h = (r[i] >> 7)&1;
            b[i] = (r[i] << 1)&0xff;
            b[i]^= h*0x1b;
        }

        r[0] = b[0]^a[3]^a[2]^b[1]^a[1];
        r[1] = b[1]^a[0]^a[3]^b[2]^a[2];
        r[2] = b[2]^a[1]^a[0]^b[3]^a[3];
        r[3] = b[3]^a[2]^a[1]^b[0]^a[0];

        // appending r[] to make 32-bit word
        let word:u64 = (r[0]<<24) | (r[1]<<16) | (r[2]<<8) | r[3];

        // making cip
        cip = (cip<<32)|word;
    }

    /* for printing purpose */
    if PRINT_DETAILS == true{
        println!("after mix nibble:\t {:x}", cip);
    }   

    return cip;
}


fn add_rk(msg:u64, key:u64) -> u64{
    let cip: u64 = msg^key;

    /* for printing purpose */
    if PRINT_DETAILS == true{
        println!("after add-rk:\t\t {:x}", cip);
    }

    return cip;
}


pub fn oracle(mut msg: u64, key: u64) -> u64{
    // generating round key from key scheduling
	let round_key = key_scheduling(key);

    /* round function */
    for i in 0..NO_OF_ROUNDS{
        /* for printing purpose */
        if PRINT_DETAILS == true{
            println!("\n********************************************************************************");
            println!("for round {}:", i);
            println!("********************************************************************************");
            }

        msg = add_rk(msg, round_key[i as usize]);
		msg = sbox(msg);
		msg = rot_nibble(msg);	
		msg = mix_nibble(msg);	
		}

    msg = add_rk(msg, round_key[NO_OF_ROUNDS as usize]);

    if PRINT_DETAILS == true{
        println!("********************************************************************************\n");
    }

    return msg;
}
