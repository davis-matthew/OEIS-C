/*
 * A256890  Triangle T(n,k) = t(n-k, k); t(n,m) = f(m)*t(n-1,m) +
 *          f(n)*t(n,m-1), where f(x) = x + 2.
 * https://oeis.org/A256890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 70, u64 n <= 197,
 *            u128 n <= 543, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A256890 – Triangle T(n,k) and sequence a(n) in row‑major order
   Formula for T(n,k):
      T(n,k) = Σ_{j=0..k} (-1)^(k-j) * binomial(j+3,j) * binomial(n+4,k-j) * (j+2)^n
   The sequence a(n) is obtained by reading T in row‑major order:
      a(n) = T(r,k)   where   r(r+1)/2 + k = n   (0 ≤ k ≤ r)   */

/* Required helper functions provided in the build environment
   (binomial, big_pow) are declared in their respective headers. */
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Compute a(n) for any non‑negative n. */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index k within that row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute T(r,k) by the closed‑form sum. */
    seq_t even_sum = 0;
    seq_t odd_sum  = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t binom1 = binomial(j + 3, j);       /* binomial(j+3,j)   */
        seq_t binom2 = binomial(r + 4, k - j);    /* binomial(r+4,k-j) */
        seq_t pow_v  = big_pow((seq_t)(j + 2), (seq_t)r); /* (j+2)^r */

        seq_t term = binom1 * binom2 * pow_v;

        if (((k - j) & 1) == 0)
            even_sum += term;
        else
            odd_sum += term;
    }

    /* even_sum ≥ odd_sum for all admissible arguments. */
    return even_sum - odd_sum;
}
