/*
 * A371067  E.g.f. satisfies A(x) = 1 + x*exp(x^2*A(x)^2).
 * https://oeis.org/A371067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/2⌋} ( (n-2k)^k * binomial(2k+1, n-2k) )
 *            / ((2k+1)*k!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Pre-cached factorials */
static seq_t *fact_cache      = NULL; /* array of factorials */
static idx_t  fact_cache_len  = 0;    /* number of elements stored (len = max_index+1) */
static size_t fact_cache_cap  = 0;    /* allocated capacity */

/* Ensure factorials up to n are cached */
static void ensure_fact_cache(idx_t n)
{
    if (n < fact_cache_len) return;

    if (fact_cache_cap < (size_t)(n + 1)) {
        size_t new_cap = fact_cache_cap ? fact_cache_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        fact_cache = tmp;
        fact_cache_cap = new_cap;
    }

    for (idx_t i = fact_cache_len; i <= n; ++i) {
        if (i == 0) fact_cache[i] = 1;
        else fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_cache_len = n + 1;
}

/* Multiplicative binomial with gcd reductions to prevent overflow */
static seq_t binom(seq_t n, seq_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (seq_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;
        seq_t den = i;

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(res, den);
        res /= g;
        den /= g;          /* den should be 1 */

        res *= num;
    }
    return res;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Cache factorials up to n */
    ensure_fact_cache(n);

    seq_t n_fact = fact_cache[n];
    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 2 * k;            /* n-2k */
        if (m == 0 && k > 0) continue;  /* (0)^k = 0 for k>0 */

        /* coeff = n! / ((2k+1)*k!) */
        seq_t coeff = n_fact / fact_cache[k];
        coeff /= (seq_t)(2 * k + 1);

        /* pow = (n-2k)^k */
        seq_t pow_val = 1;
        seq_t base = (seq_t)m;
        for (idx_t i = 0; i < k; ++i) pow_val *= base;

        /* binomial term */
        seq_t bin_val = binom((seq_t)(2 * k + 1), (seq_t)m);

        /* term = coeff * pow_val * bin_val */
        seq_t term = coeff;
        term *= pow_val;
        term *= bin_val;

        sum += term;
    }
    return sum;
}
