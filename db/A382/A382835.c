/*
 * A382835  Array read by ascending antidiagonals: A(n,k) = (6*n + 1)*(12*n +
 *          1)*Product_{i=0..k-2} (9*2^i*n + 1) with k >= 2.
 * https://oeis.org/A382835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 24, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the entry at index n of the array read by
 * ascending antidiagonals of the two‑parameter function
 *   A(n,k) = (6*n+1)*(12*n+1)*∏_{i=1}^{k} (9*2^i*n + 1)      (k ≥ 0).
 * The index n (0‑based) is converted to the pair (row,col) = (n,k) by
 * locating the antidiagonal sum s with Σ_{t=0}^{s-1} (t+1) ≤ n < Σ_{t=0}^{s} (t+1)
 * and then setting
 *   row = s - (n - s(s+1)/2)
 *   col = n - s(s+1)/2 .
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the antidiagonal sum s such that n lies in its range. */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;

    /* offset of n within this antidiagonal. */
    idx_t offset = n - (s * (s + 1)) / 2;
    idx_t row = s - offset;   /* n-coordinate */
    idx_t col = offset;       /* k-coordinate */

    /* Core value: (6*row + 1)*(12*row + 1) */
    seq_t base = ((seq_t)(6 * row + 1)) * ((seq_t)(12 * row + 1));

    /* Multiply by the product over i = 1 .. col of (9*2^i*row + 1). */
    seq_t prod = 1;
    seq_t pow2 = 1;          /* will assume 2^i after each loop */
    for (idx_t i = 1; i <= col; ++i)
    {
        pow2 *= 2;                                 /* pow2 = 2^i */
        seq_t factor = (seq_t)9 * (pow2 * ((seq_t)row)) + 1;
        prod *= factor;
    }

    return base * prod;
}
