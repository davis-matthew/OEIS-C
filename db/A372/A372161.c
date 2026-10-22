/*
 * A372161  E.g.f. A(x) satisfies A(x) = exp( 3 * x / (1 - x * A(x)^(1/3)) ).
 * https://oeis.org/A372161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * n! * Sum_{k=1..n} (n-k+3)^(k-1) * binom(n-1, k-1) /
 *            k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, ensure_cache, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A372161: a(0)=1, a(1)=3, a(2)=15, a(3)=117, ... */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"
#include "../utils/big_pow.h"
#include "../utils/factorial.h"

/* cache for computed a(n) values */
static seq_t *a_cache  = NULL;   /* memoised a(n) values */
static idx_t  a_cap    = 0;

/* factorial computed on the fly */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        ensure_cache(1, &a_cache, &a_cap);
        if (a_cache[0] == 0)
            a_cache[0] = 1;
        return a_cache[0];
    }

    /* return cached value if available */
    ensure_cache(n + 1, &a_cache, &a_cap);
    if (a_cache[n] != 0)
        return a_cache[n];

    /* compute n! once */
    seq_t fac_n = factorial(n);

    /* initial values for the iteration */
    seq_t binom = 1;                /* C(n-1,0)      */
    seq_t F     = fac_n;            /* n! / 1!      */
    seq_t sum   = 0;
    const idx_t N = n;

    for (idx_t k = 1; k <= N; ++k) {
        /* base = n - k + 3, exponent = k-1 */
        seq_t base = (seq_t)(N - k + 3);
        idx_t  exp = k - 1;
        seq_t pow_val = big_pow(base, (seq_t)exp);

        /* term = 3 * F * binom * base^(k-1) */
        seq_t term = 3 * F * binom;
        term = term * pow_val;

        sum += term;

        if (k == N)
            break;

        /* update F: divide by k+1  (F = n! / (k+1)!) */
        F = F / ((seq_t)(k + 1));

        /* update binom: binom = C(n-1,k) = binom * (n - k) / k */
        binom = binom * (seq_t)(N - k);
        binom = binom / (seq_t)k;
    }

    a_cache[n] = sum;
    return sum;
}
