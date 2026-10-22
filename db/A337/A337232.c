/*
 * A337232  Even composite integers m such that F(m)^2 == 1 (mod m), where
 *          F(m) is the m-th Fibonacci number.
 * https://oeis.org/A337232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 2483, u32 n <= 3531, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337232: even composite integers m such that F(m)^2 ≡ 1 (mod m),
   where F(m) is the m‑th Fibonacci number.  The function returns the
   n‑th term of the sequence (1‑based index).  The values are stored
   lazily; subsequent calls reuse cached results. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

/* ---- Fenwick helper : fast doubling Fibonacci modulo m ---- */
static seq_t fib_mod(idx_t n, seq_t mod)
{
    /* Returns F(n) mod mod.  Uses the fast‑doubling algorithm. */
    if (n == 0) return 0 % mod;
    seq_t a = 0 % mod;   /* F(0) */
    seq_t b = 1 % mod;   /* F(1) */

    /* Count bits of n (n > 0) */
    unsigned long long nn = (unsigned long long)n;
    int bit = 63 - __builtin_clzll(nn);

    for (int i = bit; i >= 0; --i) {
        seq_t a_old = a;
        seq_t b_old = b;

        /* c = F(2k)   = F(k) * [2*F(k+1) – F(k)] */
        seq_t tmp = (b_old * 2) % mod;
        tmp = (tmp + mod - a_old) % mod;       /* 2*F(k+1) – F(k) */
        seq_t c = (a_old * tmp) % mod;         /* F(2k) */

        /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
        seq_t d = (a_old * a_old) % mod;
        d = (d + (b_old * b_old) % mod) % mod;

        if (((nn >> i) & 1ULL) == 0) {
            a = c;
            b = d;
        } else {
            a = d;
            b = (c + d) % mod;
        }
    }
    return a % mod;
}

/* ---- Cache for found terms ---- */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t newcap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (cache) {
            memcpy(tmp, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_cap = newcap;
    }
}

/* ---- Main function required by the build ---- */
seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; for n == 0 we return 0. */
    if (n == 0) return 0;

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* Start searching from the next integer after the last found term. */
    idx_t m_start = (cache_len == 0) ? 4 :
                    (idx_t)(cache[cache_len - 1] + 1);   /* implicit cast to idx_t */

    for (idx_t m = m_start; ; ++m) {
        /* only even composite numbers */
        if ((m & 1ULL) == 0 && m > 2 && !is_prime(m)) {
            seq_t mod = (seq_t)m;
            seq_t fn   = fib_mod(m, mod);
            seq_t sq   = (fn * fn) % mod;
            if (sq == 1) {
                ensure_cache_capacity();
                cache[cache_len++] = (seq_t)m;
                if ((size_t)n <= cache_len) {
                    return cache[n - 1];
                }
            }
        }
    }
}
