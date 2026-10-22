/*
 * A309444  The successive approximations up to 5^n for 5-adic integer
 *          4^(1/3).
 * https://oeis.org/A309444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 4, a(n) = a(n-1) + 3 * (a(n-1)^3 - 4) (mod 5^n)
 *            for n > 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply two seq_t values modulo mod using __int128 when the
   underlying type is 8‑bytes or less, otherwise just use the
   built‑in operators (sufficient for arbitrary‑precision types). */
static inline seq_t mul_mod(seq_t x, seq_t y, seq_t mod) {
    if (sizeof(seq_t) <= 8) {
        return (seq_t)(((__uint128_t)x * y) % mod);
    } else {
        return (x * y) % mod;
    }
}

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *cache = NULL;     /* array of previously computed a(i) */
    static size_t cap   = 0;        /* capacity of cache array   */
    static idx_t last   = 1;        /* highest index that has been cached ( >0 ) */
    static seq_t mod_pow5 = (seq_t)5;      /* current 5^last (modulus for a(last)) */

    /* one‑time initialisation */
    if (cache == NULL) {
        /* start with space for a(0) and a(1) */
        cap   = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        mod_pow5 = (seq_t)5;   /* 5¹, modulus for a(1) */
        last      = 1;
    }

    /* trivial cases */
    if (n == 0) return cache[0];
    if (n <= last) return cache[n];

    /* grow cache array if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* compute successive terms up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        mod_pow5 *= (seq_t)5;             /* 5^k  */

        seq_t a_prev = cache[k - 1];

        /* t = a_prev^3 (mod mod_pow5) */
        seq_t t = mul_mod(a_prev, a_prev, mod_pow5);
        t = mul_mod(t, a_prev, mod_pow5);

        /* sub = (t - 4) mod mod_pow5  (handle wrap‑around) */
        seq_t sub = (t >= (seq_t)4) ? (t - (seq_t)4)
                                    : (t + mod_pow5 - (seq_t)4);

        /* diff = 3 * sub  (mod mod_pow5) */
        seq_t diff = mul_mod(sub, (seq_t)3, mod_pow5);

        /* new_val = (a_prev + diff) mod mod_pow5 */
        seq_t new_val = (a_prev + diff) % mod_pow5;

        cache[k] = new_val;
    }

    last = n;
    return cache[n];
}
