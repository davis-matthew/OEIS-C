/*
 * A269432  Number of length-n 0..5 arrays with no repeated value greater than
 *          the previous repeated value.
 * https://oeis.org/A269432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a linear recurrence a(n) = 30*a(n-1) - 369*a(n-2)
 *            + 2350*a(n-3) - 7890*a(n-4) + 11550*a(n-5) + 895*a(n-6) -
 *            11550*a(n-7) - 7890*a(n-8) - 2350*a(n-9) - 369*a(n-10) -
 *            30*a(n-11) - a(n-12), with initial 12 terms (1‑based): 6, 36,
 *            216, 1281, 7536, 44021, 255576, 1475871, 8482276, 48543...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const int coeff[12] =
{
    30,   -369,   2350,  -7890,   11550,    895,
   -11550, -7890,  -2350,   -369,   -30,     -1
};

static const seq_t init[13] =
{
    (seq_t)0ULL, /* unused, 0 index */
    (seq_t)6ULL, (seq_t)36ULL, (seq_t)216ULL,
    (seq_t)1281ULL, (seq_t)7536ULL, (seq_t)44021ULL,
    (seq_t)255576ULL, (seq_t)1475871ULL,
    (seq_t)8482276ULL, (seq_t)48543777ULL,
    (seq_t)276756312ULL, (seq_t)1572394697ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0ULL;  /* for safety if called with 0 */

    static seq_t *cache = NULL;   /* 1‑based array: cache[0] unused */
    static size_t cap = 0;        /* allocated capacity (n+1) */
    static idx_t maxComputed = 0; /* highest n already computed */

    /* Resize cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t newCap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) return (seq_t)0ULL; /* allocation error */
        cache = tmp;
        cap = newCap;
    }

    /* Compute values from maxComputed+1 up to n */
    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[i] = init[i];
        } else {
            /* Use different strategy depending on the width of seq_t. */
            if (sizeof(seq_t) <= 8) { /* 64‑bit builds: use __int128 for safety */
                __int128 sum = 0;
                for (int k = 0; k < 12; ++k) {
                    __int128 term = (__int128)cache[i - (k + 1)] *
                                    (__int128)coeff[k];
                    sum += term;
                }
                cache[i] = (seq_t)sum;
            } else { /* arbitrary‑precision builds */
                seq_t sum_pos = 0;
                seq_t sum_neg = 0;
                for (int k = 0; k < 12; ++k) {
                    seq_t term = cache[i - (k + 1)] *
                                 (seq_t)(coeff[k] >= 0 ? coeff[k] : -coeff[k]);
                    if (coeff[k] >= 0)
                        sum_pos += term;
                    else
                        sum_neg += term;
                }
                /* The recurrence ensures sum_pos >= sum_neg */
                cache[i] = sum_pos - sum_neg;
            }
        }
    }
    maxComputed = n;
    return cache[n];
}
