/*
 * A282255  Least k > 0 such that sigma(n) divides sigma(n+k).
 * https://oeis.org/A282255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that sigma(n) divides sigma(n + k)
 * Range:     u8 n <= 63, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for sigma values */
static seq_t *sigma = NULL;      /* array of divisor sums */
static idx_t  sigma_limit = 0;   /* current size of sigma array (max index computed) */

/* recompute sigma[1..new_limit] from scratch */
static void recompute_sigma(idx_t new_limit)
{
    seq_t *new_arr = (seq_t *)malloc((new_limit + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= new_limit; ++i)
        new_arr[i] = 0;          /* zero initialize */

    for (idx_t i = 1; i <= new_limit; ++i) {
        seq_t inc = (seq_t)i;
        for (idx_t j = i; j <= new_limit; j += i)
            new_arr[j] += inc;
    }

    free(sigma);
    sigma = new_arr;
    sigma_limit = new_limit;
}

/* ensure that sigma[1..limit] has been computed */
static void ensure_sigma_upto(idx_t limit)
{
    if (sigma_limit >= limit)
        return;

    /* double size until large enough */
    idx_t new_limit = sigma_limit ? sigma_limit * 2 : 2;
    while (new_limit < limit)
        new_limit *= 2;

    recompute_sigma(new_limit);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1, but guard against 0 */
        return 0;

    /* initial compute of sigma up to at least n */
    if (sigma == NULL)
        recompute_sigma(200000);   /* start with a reasonable size */

    ensure_sigma_upto(n);   /* guarantee sigma[n] exists */

    seq_t sigma_n = sigma[n];
    idx_t k = 1;

    while (1) {
        idx_t m = n + k;
        ensure_sigma_upto(m);
        if (sigma[m] % sigma_n == 0)
            return (seq_t)k;
        ++k;
    }
}
