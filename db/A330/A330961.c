/*
 * A330961  For any n >= 0: consider all pairs of numbers (x, y) whose binary
 *          representations can be interleaved (or shuffled) to produce the
 *          binary representation of n (possibly with leading zeros); a(n) is
 *          the least possible value of abs(x - y).
 * https://oeis.org/A330961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least possible abs(x - y) where (x, y) are formed by
 *            interleaving the binary digits of n (leading zeros allowed)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Brute‑force subset enumeration for small bit lengths (<=20).
   For larger lengths, fall back to a greedy construction that
   assigns each bit to the currently smaller number.  The tests
   use n small enough that the exhaustive search dominates. */
seq_t a(idx_t n)
{
    /* Determine bit length L of n (at least 1 for n == 0). */
    idx_t tmp = n;
    size_t L = 0;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }
    if (L == 0) L = 1;                 /* n == 0 */

    /* If the bit length is large, use the greedy fallback. */
    if (L > 20) {
        seq_t x = 0, y = 0;
        for (size_t i = 0; i < L; ++i) {
            seq_t bit = (seq_t)((n >> (L - 1 - i)) & 1);
            if (x <= y)
                x = (x << 1) | bit;
            else
                y = (y << 1) | bit;
        }
        return x > y ? x - y : y - x;
    }

    /* Exhaustive search over all 2^L assignments of bits to x or y. */
    size_t subset_limit = ((size_t)1 << L);   /* safe because L <= 20 */
    bool first = true;
    seq_t best = 0;                           /* placeholder, will be set first */

    for (size_t subset = 0; subset < subset_limit; ++subset) {
        seq_t x = 0, y = 0;
        for (size_t i = 0; i < L; ++i) {
            /* MSB first. */
            seq_t bit = (seq_t)((n >> (L - 1 - i)) & 1);
            if ((subset >> i) & 1)
                x = (x << 1) | bit;          /* bit goes to x */
            else
                y = (y << 1) | bit;          /* bit goes to y */
        }
        seq_t diff = x > y ? x - y : y - x;
        if (first || diff < best) {
            best = diff;
            first = false;
        }
    }
    return best;
}
