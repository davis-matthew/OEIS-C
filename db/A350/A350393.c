/*
 * A350393  Smallest degree of x with the largest coefficient in
 *          Product_{k=1..n} (1 + x^k).
 * https://oeis.org/A350393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n+1)/4) for n >= 10; special values for n < 10
 *            are precomputed as shown in OEIS
 * Range:     u8 n <= 15, u16 n <= 270, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* small n values computed from the OEIS list */
    static const seq_t small[10] = {0, 0, 0, 3, 3, 5, 9, 12, 18, 21};
    if (n < 10)
        return small[(size_t)n];

    /* n >= 10: use the closed‑form formula */
    seq_t prod = (seq_t)n * ((seq_t)n + 1);
    return prod / 4;
}
