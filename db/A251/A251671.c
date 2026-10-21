/*
 * A251671  a(n) = Sum_{k=0..n} C(n,k) * (2^k + 3^k)^k.
 * https://oeis.org/A251671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * (2^k + 3^k)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, binomial, pow2, pow3, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b) */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp) */
#include "../utils/pow3.h"       /* seq_t pow3(idx_t exp) */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Cache powers of 2 and 3 up to the largest n requested so far. */
    static seq_t *pow2_cache = NULL;
    static seq_t *pow3_cache = NULL;
    static size_t cache_cap = 0;

    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp2 = (seq_t *)realloc(pow2_cache, new_cap * sizeof(seq_t));
        seq_t *tmp3 = (seq_t *)realloc(pow3_cache, new_cap * sizeof(seq_t));
        if (!tmp2 || !tmp3) exit(1);     /* allocation failure */
        pow2_cache = tmp2;
        pow3_cache = tmp3;

        if (cache_cap == 0) {
            pow2_cache[0] = (seq_t)1;
            pow3_cache[0] = (seq_t)1;
            cache_cap = 1;
        }

        for (size_t i = cache_cap; i < new_cap; ++i) {
            pow2_cache[i] = seq_mul(pow2_cache[i-1], (seq_t)2);
            pow3_cache[i] = seq_mul(pow3_cache[i-1], (seq_t)3);
        }
        cache_cap = new_cap;
    }

    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* base = 2^k + 3^k */
        seq_t base = pow2_cache[k] + pow3_cache[k];

        /* term_pow = base^k */
        seq_t term_pow = big_pow(base, (seq_t)k);

        /* binom = C(n,k) */
        seq_t binom = binomial(n, k);

        /* term = binom * term_pow */
        seq_t term = seq_mul(binom, term_pow);

        sum += term;
    }

    return sum;
}
