/*
 * A245191  Successive states of one-sided one-dimensional cellular automaton
 *          using Rule 90, starting with a single ON cell, converted to
 *          decimal.
 * https://oeis.org/A245191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i≥0, i≡n (mod 2)} (binomial(2n+2, n+2+i) mod 2)·2^i
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 100,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t) */

/* Computes the n‑th term of OEIS A245191 for any seq_t implementation. */
seq_t a(idx_t n)
{
    /* N = 2n + 2  (binary representation of 2 * (n + 1)) */
    idx_t N = 2 * n + 2;
    seq_t result = 0;

    /* Parity of n determines the starting i (0 if even, 1 if odd) */
    idx_t parity = n & 1;
    /* Current power of two corresponding to that i */
    seq_t pow = pow2(parity);     /* pow = 2^parity  */

    for (idx_t i = parity; i <= n; i += 2) {
        /* D = n + 2 + i  – the “k” in Lucas' theorem */
        idx_t D = n + 2 + i;
        /* Binomial(N, D) is odd iff D contains only bits that N contains */
        if ( (D & ~N) == 0 ) {
            result |= pow;       /* set bit i of the result */
        }
        /* Next i value is i + 2, so multiply by 4 (shift left by 2) */
        pow *= (seq_t)4;
    }
    return result;
}
