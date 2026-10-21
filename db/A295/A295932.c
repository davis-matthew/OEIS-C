/*
 * A295932  Number of (not necessarily maximal) cliques in the n-Sierpinski
 *          carpet graph.
 * https://oeis.org/A295932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (13*8^n - 8*3^n + 5)/5, n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n‑th term of sequence A295932.
 *   n : index (uint64_t)
 * returns: a(n) as seq_t
 */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n = 0, return 0 for safety */
        return 0;

    seq_t pow8 = 1;   /* 8^n */
    seq_t pow3 = 1;   /* 3^n */

    for (idx_t i = 0; i < n; ++i) {
        pow8 *= 8;
        pow3 *= 3;
    }

    /* Compute (13*8^n - 8*3^n + 5)/5 using seq_t arithmetic.
     * For 64-bit builds the intermediate terms stay within range
     * for the n values that fit in uint64_t.  For arbitrary‑precision
     * builds the standard arithmetic operators are sufficient. */
    seq_t result = 13 * pow8;
    result -= 8 * pow3;
    result += 5;
    result /= 5;

    return result;
}
