/*
 * A309452  The successive approximations up to 7^n for 7-adic integer
 *          4^(1/5).
 * https://oeis.org/A309452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 2; for n > 1 * a(n) = a(n-1) + 2 * ( a(n-1)^5 -
 *            4 ) mod 7^n * * This implementation keeps a memoisation cache so
 *            calls can be in any * order and uses only the operators allowed
 *            on seq_t. For the * exponentiation we use repeated modular
 *            multiplication; intermediate * products are...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    /* multiply a * b modulo mod using 128‑bit intermediate */
    return (seq_t)(((__uint128_t)a * b) % mod);
}

/* compute base^5 mod mod (exponent is constant 5) */
static seq_t pow5_mod(seq_t base, seq_t mod)
{
    seq_t r = 1;
    for (idx_t i = 0; i < 5; ++i)
        r = mul_mod(r, base, mod);
    return r;
}

seq_t a(idx_t n)
{
    /* cache for previously computed values */
    static seq_t *cache   = NULL;        /* array of terms 0 .. cache_len-1 */
    static size_t cache_len = 0;         /* current capacity */
    static idx_t   last_n   = 0;         /* highest index computed */
    static seq_t   mod      = 1;         /* 7^last_n */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_len = 2; /* space for a(0) and a(1) */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 2;
        last_n = 1;
        mod    = 7;   /* 7^1 */
        if (n == 0) return cache[0];
        if (n == 1) return cache[1];
    } else {
        /* if requested term already known */
        if (n < cache_len) return cache[n];
    }

    /* grow cache to accommodate up to n */
    if (cache_len <= n) {
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    }

    /* compute successive terms up to n */
    while (last_n < n) {
        /* new index k = last_n + 1 */
        idx_t k = last_n + 1;

        /* new modulus = 7^(k) */
        seq_t new_mod = (seq_t)(((__uint128_t)mod * 7));

        /* compute a_(k-1)^5 mod 7^k */
        seq_t pow5 = pow5_mod(cache[last_n], new_mod);

        /* (pow5 - 4) mod 7^k */
        seq_t r;
        if (pow5 >= 4)
            r = pow5 - 4;
        else
            r = pow5 + new_mod - 4;

        /* r = 2 * (pow5 - 4) mod 7^k */
        r = mul_mod(r, 2, new_mod);

        /* a_k = a_(k-1) + r (no mod needed for the sum) */
        cache[k] = cache[last_n] + r;

        /* update state */
        last_n = k;
        mod    = new_mod;
    }

    return cache[n];
}
