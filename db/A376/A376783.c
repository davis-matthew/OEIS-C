/*
 * A376783  Expansion of 1/sqrt((1 - x^2 - x^5)^2 - 4*x^7).
 * https://oeis.org/A376783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1 / sqrt((1 - x^2 - x^5)^2 - 4*x^7)
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to G(x) = 1 / sqrt(1 - 2x^2 - 2x^5 + x^4 - 2x^7 + x^10)   */
/* The recurrence uses the relations (1 - B(x)) * G(x)^2 = 1 with            */
/* B(x) = 2x^2 - x^4 + 2x^5 + 2x^7 - x^10.  See the reasoning section in     */
/* the specification.                                                           */
#include <stddef.h>
#include <stdlib.h>

/* static cache for values computed up to the largest n seen so far */
static seq_t *a_cache = NULL;
static seq_t *s_cache = NULL;
static idx_t cache_size = 0;

/* ensure that the cache can provide terms up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (n + 1 > cache_size) {
        idx_t new_cap = n + 1;
        seq_t *new_a = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        seq_t *new_s = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        if (!new_a || !new_s) {
            /* out of memory – terminate */
            abort();
        }
        /* copy existing data */
        for (idx_t i = 0; i < cache_size; ++i) {
            new_a[i] = a_cache[i];
            new_s[i] = s_cache[i];
        }
        /* free old arrays */
        free(a_cache);
        free(s_cache);
        a_cache = new_a;
        s_cache = new_s;

        /* compute new terms */
        for (idx_t m = cache_size; m < new_cap; ++m) {
            if (m == 0) {
                a_cache[0] = 1;
                s_cache[0] = 1;
            } else {
                /* compute s[m] using B(x) coefficients:
                   s_m = 2*s_{m-2} - s_{m-4} + 2*s_{m-5} + 2*s_{m-7} - s_{m-10}
                */
                seq_t sm = 0;
                if (m >= 2) {
                    seq_t t = s_cache[m - 2];
                    t += t;                 /* 2*s_{m-2} */
                    sm += t;
                }
                if (m >= 4) {
                    sm -= s_cache[m - 4];
                }
                if (m >= 5) {
                    seq_t t = s_cache[m - 5];
                    t += t;                 /* 2*s_{m-5} */
                    sm += t;
                }
                if (m >= 7) {
                    seq_t t = s_cache[m - 7];
                    t += t;                 /* 2*s_{m-7} */
                    sm += t;
                }
                if (m >= 10) {
                    sm -= s_cache[m - 10];
                }
                s_cache[m] = sm;

                /* compute a[m] from the convolution:
                   a_m = (s_m - sum_{i=1}^{m-1} a_i * a_{m-i}) / 2
                */
                seq_t conv = 0;
                for (idx_t i = 1; i < m; ++i) {
                    conv += a_cache[i] * a_cache[m - i];
                }
                seq_t numerator = s_cache[m] - conv;
                numerator /= 2;            /* guaranteed to be even */
                a_cache[m] = numerator;
            }
        }
        cache_size = new_cap;
    }
}

/* external linkage function requested by the specification */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
