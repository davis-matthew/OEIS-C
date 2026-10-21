/*
 * A290251  a(n) is the number of parts in the integer partition having viabin
 *          number n.
 * https://oeis.org/A290251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0)=0; a(n>0)=1+number of 0’s in the binary representation of n
 * Range:     u8 n <= 9912, u16 n <= 9912, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(2n) = 1 + a(n); a(2n+1) = a(n) */
/* This implementation uses the direct binary–zero count. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t tmp   = (uint64_t)n;  /* idx_t is uint64_t, so safe cast */
    unsigned bits = 0;
    unsigned ones = 0;

    while (tmp) {
        bits += 1;
        ones  += (unsigned)(tmp & 1ULL);
        tmp >>= 1;
    }

    unsigned zeros = bits - ones;
    return (seq_t)(zeros + 1);
}
