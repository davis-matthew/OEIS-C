/*
 * A257178  Number of 3-Motzkin paths of length n with no level steps at odd
 *          level.
 * https://oeis.org/A257178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0..floor(n/2)} 3^(n-2i) * Catalan(i) *
 *            binomial(n-i,i) Recurrence used: a(0)=1, a(1)=3, for n>=2:
 *            a(n)=3^n + sum_{k=0}^{n-2} a(k)*a(n-2-k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *a_cache   = NULL;
static seq_t *pow3_cache = NULL;
static size_t   cache_cap = 0;
static idx_t    last_computed = 0;   /* number of terms already computed (0 … cache_cap-1) */

/* Ensure that a_cache[n] is computed for all n up to need. */
static void ensure_cache(idx_t need)
{
    if (last_computed > need)
        return;

    /* Resize to at least need+1 terms, doubling to keep realloc cost O(n). */
    size_t new_cap = (size_t)need + 1;
    if (new_cap < 2) new_cap = 2;
    while (new_cap < (size_t)need + 1)
        new_cap <<= 1;

    seq_t *new_a   = alloc_seq(new_cap);
    seq_t *new_pow = alloc_seq(new_cap);
    if (a_cache != NULL) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_a[i]   = a_cache[i];
        free_seq(a_cache);
    }
    if (pow3_cache != NULL) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_pow[i] = pow3_cache[i];
        free_seq(pow3_cache);
    }

    /* Compute new terms.  The previous terms are already copied. */
    for (size_t i = last_computed; i < new_cap; ++i) {
        if (i == 0) {
            new_a[i]   = (seq_t)1;
            new_pow[i] = (seq_t)1;
        } else if (i == 1) {
            new_a[i]   = (seq_t)3;
            new_pow[i] = (seq_t)3;
        } else {
            new_pow[i] = new_pow[i-1] * (seq_t)3;
            seq_t sum  = (seq_t)0;
            for (idx_t k = 0; k <= (idx_t)i-2; ++k)
                sum += new_a[k] * new_a[(idx_t)i-2-k];
            new_a[i] = new_pow[i] + sum;
        }
    }

    a_cache = new_a;
    pow3_cache = new_pow;
    cache_cap = new_cap;
    last_computed = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
