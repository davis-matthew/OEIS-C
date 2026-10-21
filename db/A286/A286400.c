/*
 * A286400  {1010->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A286400: a(n) = n-th term of the {1010->1}-transform of the Thue-Morse word A010060.
 * The transform scans the infinite Thue-Morse word (t(i)=parity of 1's in binary i)
 * left-to-right; whenever the 4-bit pattern 1010 occurs, it is replaced by a single 1
 * and the four input bits are discarded. All other bits are copied unchanged.
 *
 * The algorithm below computes the n-th output of this transformation
 * by simulating the scan up to the desired index.  It runs in O(n) time
 * and uses only integer operations. */

#include <stdint.h>
#include <stdbool.h>

/* Compute parity (oddness of number of 1-bits) of a 64‑bit integer. */
static inline int parity_uint64(uint64_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; value for n<1 is irrelevant but kept safe */
        return (seq_t)0;

    idx_t i = 0;          /* current index into the Thue–Morse word (0‑based) */
    idx_t out = 0;        /* number of transformed bits produced so far */
    seq_t val = 0;        /* value of the last produced bit */

    while (out < n) {
        int b0 = parity_uint64(i);
        int b1 = parity_uint64(i + 1);
        int b2 = parity_uint64(i + 2);
        int b3 = parity_uint64(i + 3);

        if (b0 && !b1 && b2 && !b3) {   /* 1010 pattern found */
            val   = (seq_t)1;
            i    += 4;                  /* consume the entire block */
        } else {
            val   = (seq_t)b0;          /* copy the current bit */
            i    += 1;
        }
        out += 1;
    }
    return val;
}
