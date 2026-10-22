/*
 * A376282  G.f. A(x) satisfies A(x) = (1 + 9*x*A(x)^7)^(1/3).
 * https://oeis.org/A376282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376282
   a(n) = 9^n * binomial((7*n+1)/3, n) / (7*n+1)
   Recurrence (for n ≥ 3):
     a(n) = 189*(7*n-11)*(7*n-17)*(7*n-2)*(7*n-20)*(7*n-5)*(7*n-8)*a(n-3)
            ------------------------------------------------------------------
            8*n*(n-1)*(n-2)*(4*n-5)*(2*n-1)*(4*n+1)
   Initial values: a(0)=1, a(1)=3, a(2)=54
*/
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Cached values for random‑access queries. */
static seq_t *cache      = NULL;   /* array of a(i) */
static size_t capacity   = 0;      /* allocated size */
static idx_t  lastComputed = 0;    /* highest n already stored */

seq_t a(idx_t n)
{
    /* Initialise cache with the first three terms. */
    if (cache == NULL) {
        capacity = 3;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)1U;
        cache[1] = (seq_t)3U;
        cache[2] = (seq_t)54U;
        lastComputed = 2;
    }

    if (n <= lastComputed)
        return cache[n];

    /* Grow capacity if required. */
    if ((size_t)(n + 1) > capacity) {
        size_t newCap = capacity;
        while ((size_t)(n + 1) > newCap) newCap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed */
        cache = tmp;
        capacity = newCap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = lastComputed + 1; i <= n; ++i) {
        if (i < 3) { /* safety, though i ≥ 3 here */
            cache[i] = (i==0? (seq_t)1U : (i==1? (seq_t)3U : (seq_t)54U));
            continue;
        }

        /* Numerator factors: a(i-3), 189, 7*i-11, 7*i-17, 7*i-2, 7*i-20, 7*i-5, 7*i-8 */
        seq_t num[8];
        num[0] = cache[i-3];
        num[1] = (seq_t)189;
        seq_t seven_i = (seq_t)7 * (seq_t)i;
        num[2] = seven_i - (seq_t)11;
        num[3] = seven_i - (seq_t)17;
        num[4] = seven_i - (seq_t)2;
        num[5] = seven_i - (seq_t)20;
        num[6] = seven_i - (seq_t)5;
        num[7] = seven_i - (seq_t)8;

        /* Denominator factors: 8*i, i-1, i-2, 4*i-5, 2*i-1, 4*i+1 */
        seq_t den[6];
        den[0] = (seq_t)8 * (seq_t)i;
        den[1] = (seq_t)(i-1);
        den[2] = (seq_t)(i-2);
        den[3] = ((seq_t)4 * (seq_t)i) - (seq_t)5;
        den[4] = ((seq_t)2 * (seq_t)i) - (seq_t)1;
        den[5] = ((seq_t)4 * (seq_t)i) + (seq_t)1;

        /* first cancel common factors between the base term a(i-3) and denominators */
        for (size_t j = 0; j < 6; ++j) {
            seq_t g = gcd(num[0], den[j]);   /* num[0] is a(i-3) */
            if (g > (seq_t)1) {
                num[0] /= g;
                den[j] /= g;
            }
        }

        /* collect remaining denominators */
        seq_t remDen[6];
        size_t remCnt = 0;
        for (size_t j = 0; j < 6; ++j) {
            if (den[j] != (seq_t)1)
                remDen[remCnt++] = den[j];
        }

        /* cancel remaining denominators with all numerator factors (except a(i-3) now stored in num[0]) */
        for (size_t k = 1; k < 8; ++k) {
            for (size_t j = 0; j < remCnt; ++j) {
                seq_t g = gcd(num[k], remDen[j]);
                if (g > (seq_t)1) {
                    num[k] /= g;
                    remDen[j] /= g;
                }
            }
        }

        /* try to cancel any remaining denominators with the base term a(i-3) again */
        for (size_t j = 0; j < remCnt; ++j) {
            seq_t g = gcd(num[0], remDen[j]);
            if (g > (seq_t)1) {
                num[0] /= g;
                remDen[j] /= g;
            }
        }

        /* Now all remaining denominators should divide the product exactly.
           Multiply numerators together. */
        seq_t result = num[0];        /* start with a(i-3) */
        for (size_t k = 1; k < 8; ++k) {
            if (num[k] != (seq_t)1)
                result = result * num[k];
        }

        /* finally divide by any remaining denominators (should all be 1). */
        for (size_t j = 0; j < remCnt; ++j) {
            if (remDen[j] != (seq_t)1)
                result = result / remDen[j];
        }

        cache[i] = result;
    }

    lastComputed = n;
    return cache[n];
}
