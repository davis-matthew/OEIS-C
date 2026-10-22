/*
 * A400418  a(n) = Sum_{k=1..n} A007947(floor(n/k)).
 * https://oeis.org/A400418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = Sum_{k=1..n} A007947(floor(n/k))
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007947(n) is the radical of n (product of distinct prime factors of n).  */

#include <stdlib.h>
#include <stdint.h>

static seq_t *rad = NULL;          /* rad[1..n], 1‑indexed */
static idx_t  precomp_n = 0;      /* maximum n for which rad is computed */

/* Ensure that rad[1..n] is filled; recompute if a larger n is requested.  */
static void ensure_rad(idx_t n)
{
    if (n <= precomp_n)
        return;          /* already sufficient */

    /* free old array if any */
    if (rad != NULL) {
#ifdef __cplusplus
        delete[] rad;
#else
        free(rad);
#endif
    }

#ifdef __cplusplus
    rad = new seq_t[n + 1]();          /* value‑initialized   */
#else
    rad = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t)); /* zero initialized */
#endif
    if (rad == NULL)
        exit(1);                 /* allocation failed */

    /* smallest prime factor array for 1..n */
    uint32_t *spf = NULL;
#ifdef __cplusplus
    spf = new uint32_t[n + 1]();          /* zero‑initialized */
#else
    spf = (uint32_t *)calloc((size_t)n + 1, sizeof(uint32_t));
#endif
    if (spf == NULL)
        exit(1);

    /* Sieve to fill spf */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {              /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* Compute radical for each m */
    rad[1] = (seq_t)1;                 /* rad(1)=1 */
    for (idx_t m = 2; m <= n; ++m) {
        uint32_t p = spf[m];            /* smallest prime factor of m */
        idx_t rest = m / p;
        if (rest % p == 0) {            /* p already counted in rest */
            rad[m] = rad[rest];
        } else {
            rad[m] = rad[rest] * (seq_t)p;
        }
    }

    /* free spf array */
#ifdef __cplusplus
    delete[] spf;
#else
    free(spf);
#endif

    precomp_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* graceful handling of n==0 */

    ensure_rad(n);

    seq_t sum = (seq_t)0;
    for (idx_t t = 1; t <= n; ++t) {
        /* number of k such that floor(n/k) == t */
        idx_t cnt = n / t - n / (t + 1);
        sum += rad[t] * (seq_t)cnt;
    }
    return sum;
}
