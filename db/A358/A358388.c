/*
 * A358388  a(n) = hypergeom([n, -n, 1/2], [1, 1], -8).
 * https://oeis.org/A358388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(k-1) * binomial(2*k, k)^2 * * (binomial(n
 *            + k, 2*k) + binomial(n + k - 1, 2*k))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (Offset: 0) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(a,b) */
#include "../utils/pow2.h"       /* pow2(exp) */

static idx_t max_computed = 0;
static seq_t *a_cache  = NULL;   /* a(n) values */
static size_t a_cap = 0;

static seq_t *B        = NULL;   /* C(n + k, 2*k) */
static seq_t *B2       = NULL;   /* C(n + k - 1, 2*k) */
static size_t b_cap = 0;

static seq_t *f        = NULL;   /* f[k] = 2^(k-1) * binomial(2*k, k)^2 */
static size_t f_cap = 0;

static void grow_cache(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = (need < 16) ? 16 : need * 2;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap   = new_cap;
}

static void grow_binom_arrays(size_t need)
{
    if (need <= b_cap) return;
    size_t new_cap = (need < 16) ? 16 : need * 2;
    B  = (seq_t *)realloc(B,  new_cap * sizeof(seq_t));
    B2 = (seq_t *)realloc(B2, new_cap * sizeof(seq_t));
    b_cap = new_cap;
}

static void grow_f_array(size_t need)
{
    if (need <= f_cap) return;
    size_t new_cap = (need < 16) ? 16 : need * 2;
    f = (seq_t *)realloc(f, new_cap * sizeof(seq_t));
    f_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Ensure caches are big enough. */
    if (n > max_computed) {
        size_t need = n + 1;
        grow_cache(need);
        grow_binom_arrays(need);
        grow_f_array(need);

        /* First call: initialise indices 0. */
        if (max_computed == 0) {
            B[0]   = (seq_t)1;
            B2[0]  = (seq_t)1;
            a_cache[0] = (seq_t)1;
        }

        /* Compute all values up to n. */
        for (idx_t cur = max_computed + 1; cur <= n; ++cur) {

            /* Update existing k-values (k = 1 .. cur-1). */
            for (idx_t k = cur - 1; k >= 1; --k) {
                /* B_k = C(cur + k, 2k) */
                seq_t numB  = (seq_t)(cur + k);
                seq_t denB  = (seq_t)(cur - k);
                B[k] = B[k] * numB / denB;

                /* B2_k = C(cur - 1 + k, 2k) */
                if (cur - 1 - k == 0) {
                    /* Special case where denominator would be 0:
                       C(2k, 2k) = 1.  Previously B2[k] was 0. */
                    B2[k] = (seq_t)1;
                } else {
                    seq_t numB2 = (seq_t)(cur - 1 + k);
                    seq_t denB2 = (seq_t)(cur - 1 - k);
                    B2[k] = B2[k] * numB2 / denB2;
                }
            }

            /* New k = cur */
            B[cur]  = binomial((idx_t)(cur + cur), (idx_t)(2 * cur));
            B2[cur] = binomial((idx_t)(cur + cur - 1), (idx_t)(2 * cur));

            /* Compute f[cur] = 2^(cur-1) * binomial(2*cur, cur)^2 */
            if (cur >= 1) {
                seq_t bin   = binomial((idx_t)(2 * cur), (idx_t)cur);
                seq_t sq    = bin * bin;
                seq_t pow   = pow2((idx_t)(cur - 1));
                f[cur] = sq * pow;
            }

            /* Sum the series for this n. */
            seq_t sum = (seq_t)1;   /* k = 0 term */
            for (idx_t k = 1; k <= cur; ++k) {
                sum += f[k] * (B[k] + B2[k]);
            }
            a_cache[cur] = sum;
        }
        max_computed = n;
    }

    return a_cache[n];
}
