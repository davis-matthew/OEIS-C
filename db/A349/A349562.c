/*
 * A349562  Number of labeled rooted forests with 2-colored leaves.
 * https://oeis.org/A349562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*(k+1)^(n-1) (OEIS A349562)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, mul_small, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

static seq_t *pow_cache = NULL;          /* (k+1)^(current_n-1) for k=0..current_n */
static seq_t *a_cache  = NULL;          /* a(n) values computed so far */
static size_t cap_pow = 0;              /* capacity of pow_cache */
static size_t cap_a  = 0;               /* capacity of a_cache  */
static idx_t  max_n  = 0;               /* highest n computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* a(0) = 1 */

    /* enlarge caches if needed */
    if ((size_t)n >= cap_pow || (size_t)n >= cap_a) {
        size_t new_cap = (size_t)n + 1;
        if (cap_pow < new_cap) {
            pow_cache = seq_realloc(pow_cache, cap_pow, new_cap);
            cap_pow = new_cap;
        }
        if (cap_a  < new_cap) {
            a_cache  = seq_realloc(a_cache,  cap_a , new_cap);
            cap_a  = new_cap;
        }
    }

    /* compute new values up to n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        if (cur == 1) {                  /* initialize for n = 1 */
            pow_cache[0] = (seq_t)1;     /* 1^0 */
            pow_cache[1] = (seq_t)1;     /* 2^0 */
        } else {
            /* update existing powers to the new exponent (cur-1) */
            for (idx_t k = 0; k < cur; ++k) {
                pow_cache[k] = mul_small(pow_cache[k], (unsigned int)(k + 1));
            }
            /* new element for k = cur */
            pow_cache[cur] = big_pow((seq_t)(cur + 1), (seq_t)(cur - 1));
        }

        /* compute a(cur) = sum_{k=0..cur} C(cur,k) * pow_cache[k] */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(cur,0) */
        for (idx_t k = 0; k <= cur; ++k) {
            seq_t term = seq_mul(binom, pow_cache[k]);
            sum += term;                 /* seq_t supports += */

            if (k < cur) {
                binom = mul_small(binom, (unsigned int)(cur - k));
                binom = binom / (k + 1); /* exact division by small int */
            }
        }
        a_cache[cur] = sum;
    }

    max_n = n;
    return a_cache[n];
}
