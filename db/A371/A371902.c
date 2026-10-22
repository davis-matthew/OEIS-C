/*
 * A371902  Positive integers whose binary form follows the periodic pattern
 *          1101110: the concatenation of halftones 2 2 1 2 2 2 1, diminished
 *          by one, between successive pitches in the Ionian Major Scale.
 * https://oeis.org/A371902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(110/127 * 2^n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Use exact integer arithmetic without overflow for 64‑bit
       seq_t.  For larger seq_t types (arbitrary‑precision) we fall
       back to big_pow. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit maths handles n up to 127 safely.  For larger n
           the result would not fit in seq_t anyway. */
        if (n >= 128) {
            /* Value will overflow 64‑bit; just return 0 (any value
               is fine for out‑of‑range n). */
            return (seq_t)0;
        }
        unsigned __int128 pow2 = ((unsigned __int128)1) << n;
        unsigned __int128 prod  = (unsigned __int128)110 * pow2;
        return (seq_t)(prod / (unsigned __int128)127);
    } else {
        /* seq_t has enough capacity (arbitrary‑precision).  Use
           big_pow for exact 2^n. */
        seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
        seq_t prod = (seq_t)110 * pow2;
        return prod / (seq_t)127;
    }
}
