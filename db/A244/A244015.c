/*
 * A244015  Denominators of rational approximations to sqrt(6) obtained from
 *          Newton's method.
 * https://oeis.org/A244015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244015: Denominators of rational approximations to sqrt(6) via Newton's method
   Recurrence: start with p=2, q=1 (x0=2). For n≥0
     num = p*p + 6*q*q
     den = 2*p*q
     g   = gcd(num, den)
     p   = num / g
     q   = den / g
   a(n) = q (the denominator after the n‑th iteration) */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Euclidean algorithm for seq_t (works for built‑in and arbitrary‑precision integers) */
static inline seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
#ifdef __cplusplus
    /* C++: use std::vector to handle arbitrary‑precision types with proper construction */
    static std::vector<seq_t> cache;
    static seq_t p = 0, q = 0;

    if (cache.empty()) {          /* first call: initialise idiom */
        p = (seq_t)2;
        q = (seq_t)1;
        cache.push_back(q);        /* a(0) = 1 */
    }

    /* compute up to n (inclusive) */
    for (idx_t i = (idx_t)cache.size(); i <= n; ++i) {
        seq_t S  = (seq_t)6;
        seq_t p2 = p * p;
        seq_t q2 = q * q;
        seq_t num = p2 + S * q2;
        seq_t den = (seq_t)2 * p * q;
        seq_t g   = seq_gcd(num, den);
        seq_t denom = den / g;
        p = num / g;
        q = denom;
        cache.push_back(q);        /* store a(i) = q */
    }
    return cache[(size_t)n];
#else
    /* C: use malloc/realloc; assumes seq_t is a trivially constructible type */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored (next free index) */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t p = 0, q = 0;

    /* initialisation */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        p = (seq_t)2;
        q = (seq_t)1;
        cache[0] = q;             /* a(0) = 1 */
        cache_len = 1;
    }

    /* grow array if necessary */
    while (cache_len <= (size_t)n) {
        if (cache_cap <= cache_len) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 1;
            void *tmp = realloc(cache, sizeof(seq_t) * new_cap);
            cache = (seq_t *)tmp;
            cache_cap = new_cap;
        }

        seq_t S  = (seq_t)6;
        seq_t p2 = p * p;
        seq_t q2 = q * q;
        seq_t num = p2 + S * q2;
        seq_t den = (seq_t)2 * p * q;
        seq_t g   = seq_gcd(num, den);
        seq_t denom = den / g;
        p = num / g;
        q = denom;
        cache[cache_len] = q;      /* store a(cache_len) = q */
        ++cache_len;
    }
    return cache[(size_t)n];
#endif
}
