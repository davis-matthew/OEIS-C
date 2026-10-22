/*
 * A302606  a(n) = n! * [x^n] exp(n*x)*arcsinh(x).
 * https://oeis.org/A302606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k odd} (-1)^((k-1)/2) * binomial(n,k) * ((k-2)!!)^2
 *            * n^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* implementation uses O(n) time and O(n) temporary storage */

#include <stdlib.h>
#include <stddef.h>

/* static cache for powers of n */
static seq_t *pow_cache = NULL;
static size_t pow_cap = 0;

/* ensure that pow_cache has at least size elements */
static void ensure_pow_cache(size_t needed)
{
    if (pow_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(pow_cache, needed * sizeof(seq_t));
        if (!tmp && needed > 0) {
            /* allocation failed – abort, caller will handle */
            pow_cache = NULL;
            pow_cap = 0;
            return;
        }
        pow_cache = tmp;
        pow_cap = needed;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* precompute powers of n: pow_cache[i] = n^i for i=0..n */
    ensure_pow_cache(n + 1);
    pow_cache[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        pow_cache[i] = pow_cache[i - 1] * (seq_t)n;
    }

    /* iterate over k, computing binomial(n,k) incrementally */
    seq_t binom = 1;          /* binomial(n,0) */
    seq_t d = 1;              /* (k-2)!! for first odd k=1 */
    seq_t d_sq = 1;           /* d^2 */
    int odd_index = 0;        /* (k-1)/2 for current odd k */
    seq_t pos = 0, neg = 0;   /* accumulate positive and negative parts */

    for (idx_t k = 1; k <= n; ++k) {
        /* update binomial coefficient */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        if (k & 1) {          /* k is odd */
            seq_t term = binom;
            term *= d_sq;
            term *= pow_cache[n - k];

            if ((odd_index & 1) == 0) {   /* sign = + */
                pos += term;
            } else {                     /* sign = - */
                neg += term;
            }

            /* prepare for next odd k (k + 2) */
            odd_index++;
            d *= (seq_t)(2 * odd_index - 1);
            d_sq = d * d;
        }
    }

    /* result = positive part – negative part (guaranteed non‑negative) */
    return pos - neg;
}
