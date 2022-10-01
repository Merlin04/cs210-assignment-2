#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../libcompact.h"

/*
 * For this assignment you will be dealing with Tiny Floating Point numbers...
 * 8 bits.
 * - The MSB is the sign bit
 * - The significand representation is the 3 least significant bits
 * - The remaining 4 bits are the exponent representation
 *
 * For example, the number 2.5 would be represented as:
 * 0 1 0 0 0 0 1 0
 * _ _______ _____
 * sign
 *   exponent
 *           significand
 *
 * Bias is 7
 *
 * Decoding algorithm: (-1)^sign * 1.[significand] * 2^(exponent - bias)
 * So above example = (-1)^0 * 1.25 * 2^(8-7)
 *                             (01b = 2^(-2), and 2 = 01b reversed)
 *                  = 1 * 1.25 * 2
 *                  = 2.5
 */

const I8 BIAS = 7;

// Don't worry, not used in my representation
typedef struct {
    U8 sign;
    U8 exp;
    U8 sig;
} UnpackedTFP;

UnpackedTFP unpack(U8 a) {
    UnpackedTFP u;
    u.sign = a >> 7;
    u.exp = ((a & 0b01111000) >> 3);
    u.sig = (a & 0b00000111);
    return u;
}

U8 pack(UnpackedTFP* u) {
    // debug: make sure each value falls within ranges
    if((u->sign & 0b00000001) != u->sign) {
        P("Sign bit out of range: %d\n", u->sign);
    }
    if((u->exp & 0b00001111) != u->exp) {
        P("Exponent out of range: %d\n", u->exp);
    }
    if((u->sig & 0b00000111) != u->sig) {
        P("Significand out of range: %d\n", u->sig);
    }
    U8 a = 0;
    a |= u->sign << 7;
    a |= u->exp << 3;
    a |= u->sig;
    return a;
}

/*
 * Addition algorithm (from PPT)
 * - (-1)^sign1 * sig1 * 2^exp1 + (-1)^sign2 * sig2 * 2^exp2
 *    - (assume WLOG exp1 >= exp2)
 * - Exact Result: (-1)^sign * sig * 2^exp
 *    - Sign and sig: result of signed align and add
 *    - Exp: exp1
 * - Fixing
 *    - While sig >= 2, shift sig right, increment exp
 *    - While sig < 1, shift sig left, decrement exp
 *    - if exp out of range (0-15), overflow
 *    - Round sig to fit frac precision (3 bits)
 */

// Using the addition algorithm above, add two "Tiny Floating Point" numbers.
U8 add_tiny_fp(U8 a, U8 b) {
    // First, unpack the numbers
    UnpackedTFP u1 = unpack(a);
    UnpackedTFP u2 = unpack(b);
    // Match/align exponents - find smaller one, make it match bigger one
    if (u1.exp < u2.exp) {
        u1.sig >>= (u2.exp - u1.exp);
        u1.exp = u2.exp;
    } else if (u2.exp < u1.exp) {
        u2.sig >>= (u1.exp - u2.exp);
        u2.exp = u1.exp;
    }
    // Now, add the significands
    UnpackedTFP res = {
        .sign = u1.sign,
        .exp = u1.exp,
        .sig = u1.sig + u2.sig
    };
    // Fixing (aka normalization)
    while (res.sig >= 8) {
        res.sig >>= 1;
        res.exp++;
    }
    while (res.sig < 4) {
        res.sig <<= 1;
        res.exp--;
    }
    // Check for overflow
    if (res.exp > 15) {
        res.sign = 0;
        res.exp = 0b1111;
        res.sig = 0;
    }
    // Round sig to fit frac precision (3 bits)
    // using the nearest even rounding method
    if (res.sig & 0b1000) {
        res.sig += 0b1000;
        res.sig >>= 1;
    }

    // Pack the result and return it
    return pack(&res);
}

/*
 * Addition algorithm (from PPT)
 * - (-1)^sign1 * sig1 * 2^exp1 * (-1)^sign2 * sig2 * 2^exp2
 * - Exact Result: (-1)^sign * sig * 2^exp
 *    - Sign: s1 ^ s2
 *    - Sig: sig1 * sig2
 *    - Exp: exp1 + exp2
 * - Fixing
 *    - While sig >= 2, shift sig right, increment exp
 *    - if exp out of range (0-15), overflow
 *    - Round sig to fit frac precision (3 bits)
 * - Implementation
 *    - Biggest chore is multiplying significands
 */

// Using the multiplication algorithm above, multiply two "Tiny Floating Point" numbers.
// I couldn't get this code to work, probably some weird aspect of floating point numbers I'm missing (it's 11 pm and I'm tired)
U8 mult_tiny_fp(U8 a, U8 b) {
    // First, unpack the numbers
    UnpackedTFP u1 = unpack(a);
    UnpackedTFP u2 = unpack(b);
    // Now, multiply the significands
    // this doesn't work like regular multiplication, since it's binary and not decimal
    // and since it's really 1.[the significand]
    // so we have to do it manually
    UnpackedTFP res = {
        .sign = u1.sign ^ u2.sign,
        .exp = u1.exp + u2.exp,
        .sig = 0
    };

// first flip the order of the bits in the significand (like abc -> cba)
// then multiply the significands
// then flip them back
    u1.sig = ((u1.sig & 0b010) >> 1) | ((u1.sig & 0b001) << 1);
    u2.sig = ((u2.sig & 0b010) >> 1) | ((u2.sig & 0b001) << 1);

    for(int i = 0; i < 3; i++) {
        if(u1.sig & (1 << i)) {
            for(int j = 0; j < 3; j++) {
                if(u2.sig & (1 << j)) {
                    res.sig += 1 << (i + j);
                }
            }
        }
    }

    // now flip the entire significand back (all 8 bits)
    res.sig = ((res.sig & 0b01000000) >> 6) | ((res.sig & 0b00100000) >> 4) | ((res.sig & 0b00010000) >> 2) | ((res.sig & 0b00001000)) | ((res.sig & 0b00000100) << 2) | ((res.sig & 0b00000010) << 4) | ((res.sig & 0b00000001) << 6);

    // Fixing (aka normalization)
    while (res.sig >= 8) {
        res.sig >>= 1;
        res.exp++;
    }

    // Check for overflow
    if (res.exp > 15) {
        res.sign = 0;
        res.exp = 0b1111;
        res.sig = 0;
    }

    // Round sig to fit frac precision (3 bits)
    // using the nearest even rounding method
    if (res.sig & 0b1000) {
        res.sig += 0b1000;
        res.sig >>= 1;
    }

    // Pack the result and return it
    return pack(&res);
}

double tiny_fp_to_float(U8 a) {
    // this probably isn't the ideal way to do this, since it involves converting to decimal then doing floating point math
    // I could probably do it with bit hacks but this is only for testing so it's ok
    // it's also not complete (TODO?), since it doesn't handle all the edge cases that come with FP, but good enough for testing

    U8 sign = a >> 7;
    U8 exp = (a & 0b01111111) >> 3;
    U8 significand = a & 0b00000111;

    return (sign ? -1 : 1) * (1 + (double)1 / pow(2, (double)significand)) * pow(2, exp - BIAS);
}

// Test

int main() {
    P("Should be 2.5: %f\n", tiny_fp_to_float(0b01000010));
    P("Should be 1.007812: %f\n", tiny_fp_to_float(0b00111111));
    P(
        "%f + %f: %f\n",
        tiny_fp_to_float(0b00111111),
        tiny_fp_to_float(0b00111111),
        tiny_fp_to_float(add_tiny_fp(0b00111111, 0b00111111))
    );
    P("Should be %f: %f\n", 1.007812 * 1.007812, tiny_fp_to_float(mult_tiny_fp(0b00111111, 0b00111111)));
}