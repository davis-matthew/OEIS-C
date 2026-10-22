/*
 * A326031  Weight of the set-system with BII-number n.
 * https://oeis.org/A326031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum over each 1‑bit position p in n of popcount(p+1)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here popcount(x) = A000120(x) = number of 1 bits in binary form of x */
/* Thus a(n) = Σ_{p: bit p of n is 1} popcount(p+1)                      */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;          /* result accumulator */
    idx_t  pos = 0;         /* current bit position (0‑based) */

    while (n) {
        if (n & 1ULL) {     /* if the least significant bit is set */
            /* popcount of (pos + 1);  pos+1 <= 64 so builtin is safe */
            sum += (seq_t)__builtin_popcountll(pos + 1);
        }
        n >>= 1;            /* shift to next bit */
        ++pos;
    }
    return sum;
}
