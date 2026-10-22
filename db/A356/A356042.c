/*
 * A356042  a(n) = Sum_{k=1..n} sigma_2(k) * floor(n/k).
 * https://oeis.org/A356042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} σ₂(k) * ⌊n/k⌋, where σ₂(k)=Σ_{d|k} d²
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build environment. */
/* The function is called for any n ≥ 1. */
#include <stdlib.h>  /* malloc, free, abort */

seq_t a(idx_t n)
{
    if (n == 0)   /* not used, but keeps the function well‑defined */
        return 0;

    /* Allocate an array for σ₂(1..n).  The array is zero‑initialized. */
    seq_t *sigma = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!sigma) abort();
    for (idx_t i = 0; i <= n; ++i)
        sigma[i] = 0;

    /* Compute σ₂ by a classic divisor sieve: for each i add i² to
       all multiples of i. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i2 = (seq_t)i * (seq_t)i;            /* i² in seq_t domain */
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += i2;
    }

    /* Accumulate the required sum. */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k)
        result += sigma[k] * (seq_t)(n / k);

    free(sigma);
    return result;
}
