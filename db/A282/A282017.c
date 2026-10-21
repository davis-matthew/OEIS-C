/*
 * A282017  Coefficients in q-expansion of (E_4 + E_2^2)/2, where E_2 and E_4
 *          are the Eisenstein series shown in A006352 and A004009,
 *          respectively.
 * https://oeis.org/A282017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 120*σ_3(n) - 24*σ_1(n) + 288*∑_{k=1}^{n-1} σ_1(k)σ_1(n-k)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n‑th term of A282017, the coefficients in the
   q‑expansion of (E_4 + E_2^2)/2, where σ_1 and σ_3 are the sum of
   divisors functions. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* allocate arrays for divisor sums up to n */
    size_t len = (size_t)n + 1;   /* indices 0..n */
    seq_t *sigma1 = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *sigma3 = (seq_t *)calloc(len, sizeof(seq_t));
    if (!sigma1 || !sigma3)
        return (seq_t)0;          /* allocation failed → return 0 */

    /* compute σ₁ and σ₃ via a simple divisor sieve */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii  = (seq_t)i;
        seq_t ii2 = ii * ii;                 /* i²  */
        seq_t ii3 = ii2 * ii;                /* i³  */
        for (idx_t j = i; j <= n; j += i) {
            sigma1[j] += ii;
            sigma3[j] += ii3;
        }
    }

    /* compute the convolution Σ_{k=1}^{n-1} σ₁(k)σ₁(n-k) */
    seq_t conv = 0;
    for (idx_t k = 1; k < n; ++k)
        conv += sigma1[k] * sigma1[n - k];

    /* assemble the final formula */
    seq_t result =
        (seq_t)120 * sigma3[n] -
        (seq_t)24  * sigma1[n] +
        (seq_t)288 * conv;

    /* free temporary storage (optional for short‑living data) */
    free(sigma1);
    free(sigma3);

    return result;
}
