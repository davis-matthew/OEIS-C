/*
 * A288703  Number of (undirected) paths in the n-barbell graph.
 * https://oeis.org/A288703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000522(n‑1)^2 + A000522(n) – n – 1 * where A000522(k) =
 *            ⌊k!·e⌋ = k·A000522(k‑1) + 1, A000522(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute A000522 values up to n */
    seq_t s_prev = 1;          /* S[0] = 1 */
    seq_t s_n_minus1 = 1;      /* will hold S[n-1] */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t s_curr = ((seq_t)i) * s_prev + 1;   /* S[i] = i·S[i‑1] + 1 */
        if (i == n - 1)
            s_n_minus1 = s_curr;                  /* capture S[n‑1] */
        s_prev = s_curr;                          /* S[i] becomes previous for next iteration */
    }

    /* after the loop: s_prev = S[n], s_n_minus1 = S[n‑1] */
    seq_t term1 = s_n_minus1 * s_n_minus1;        /* square of S[n‑1] */
    seq_t result = term1 + s_prev - n - 1;
    return result;
}
