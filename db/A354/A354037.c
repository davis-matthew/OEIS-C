/*
 * A354037  a(n) = 1 if sigma(n)-n is a multiple of 4, otherwise 0.
 * https://oeis.org/A354037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if σ(n) - n is a multiple of 4, otherwise 0
 * Range:     u8 n <= 9512, u16 n <= 9512, u32 n <= 9512, u64 n <= 9512,
 *            u128 n <= 9512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *sigma = NULL;      /* array of σ values, 1‑based */
static size_t sigma_capacity = 0;

/* Precompute σ values up to at least n */
static void ensure_sigma(idx_t n)
{
    size_t needed = (size_t)n;
    if (needed <= sigma_capacity)
        return;

    /* Allocate new array */
    size_t new_cap = needed;
    seq_t *new_sigma = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1));
    if (!new_sigma)
        exit(EXIT_FAILURE);          /* out of memory */

    /* Initialize to zero */
    for (size_t i = 0; i <= new_cap; ++i)
        new_sigma[i] = 0;

    /* Sieve to compute σ values */
    for (size_t i = 1; i <= new_cap; ++i)
    {
        seq_t add = (seq_t)i;
        for (size_t j = i; j <= new_cap; j += i)
        {
            new_sigma[j] += add;
        }
    }

    /* Replace old array */
    if (sigma)
        free(sigma);
    sigma = new_sigma;
    sigma_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* not defined for n<1, but guard */

    ensure_sigma(n);

    seq_t sig = sigma[(size_t)n];
    seq_t diff = sig - (seq_t)n;   /* σ(n) - n */

    return (diff % 4 == 0) ? 1 : 0;
}
