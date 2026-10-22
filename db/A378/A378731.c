/*
 * A378731  G.f. A(x) satisfies A(x) = ( 1 + x / (1 - x*A(x)^(4/3)) )^3.
 * https://oeis.org/A378731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Sum_{k=1..n} [ C(4*(n-k)+3 , k) * C(n-1 , k-1) /
 *            (4*(n-k)+3) ]
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom, gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of values already computed */

    /* Ensure capacity for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure – not expected */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n (inclusive) */
    for (size_t m = cache_len; m <= (size_t)n; ++m) {
        if (m == 0) {
            cache[m] = (seq_t)1;   /* a(0) = 1 */
            continue;
        }

        seq_t sum_num = (seq_t)0;   /* numerator of the running rational sum */
        seq_t sum_den = (seq_t)1;   /* denominator of the running rational sum */

        /* Sum k = 1..m */
        for (idx_t k = 1; k <= m; ++k) {
            idx_t m_curr = 4 * m - 4 * k + 3;
            seq_t denom = (seq_t)m_curr;

            seq_t binom1 = binom((idx_t)m_curr, k);           /* C(m_curr , k)   */
            seq_t binom2 = binom((idx_t)m - 1, k - 1);        /* C(m-1 , k-1)    */

            seq_t term_num = seq_mul(binom1, binom2);         /* numerator of term */
            seq_t term_den = denom;                          /* denominator of term */

            /* sum_num/sum_den + term_num/term_den */
            seq_t prod1 = seq_mul(sum_num, term_den);
            seq_t prod2 = seq_mul(term_num, sum_den);
            seq_t new_num = prod1 + prod2;
            seq_t new_den = seq_mul(sum_den, term_den);

            seq_t g = gcd(new_num, new_den);
            sum_num = new_num / g;
            sum_den = new_den / g;
        }

        /* Final value: multiply by r = 3 and divide by sum_den */
        seq_t temp = seq_mul(sum_num, (seq_t)3);
        cache[m] = temp / sum_den;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
