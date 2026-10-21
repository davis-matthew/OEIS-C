/*
 * A247937  Least integer m > n such that m + n divides F(m) + F(n), where
 *          F(k) refers to the Fibonacci number A000045(k).
 * https://oeis.org/A247937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A247937
   Formula: a(n) = least m>n such that (m+n) divides (F(m)+F(n)),
   where F(k) is the k-th Fibonacci number. */

#include <stdint.h>
#include <stdbool.h>

/* Multiplication modulo using 128‑bit arithmetic
   when it is available, otherwise plain 64‑bit multiplication.
   All arguments fit in idx_t (uint64_t).  */
static inline idx_t mul_mod_idx(idx_t a, idx_t b, idx_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (idx_t)(((__uint128_t)a * b) % mod);
#else
    /* Fallback – risk of overflow if idx_t product exceeds 64 bits. */
    return (idx_t)(((unsigned long long)a * (unsigned long long)b) % mod);
#endif
}

/* Fast doubling: return F(n) mod mod and F(n+1) mod mod. */
static inline void fib_mod_pair(idx_t n, idx_t mod, idx_t *fn, idx_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }

    idx_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);

    /* c = F(2k) = F(k)*(2*F(k+1) – F(k)) */
    idx_t two_b = mul_mod_idx(b, 2, mod);
    idx_t t     = (two_b + mod - a) % mod;
    idx_t c = mul_mod_idx(a, t, mod);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    idx_t e = mul_mod_idx(a, a, mod);
    idx_t f = mul_mod_idx(b, b, mod);
    idx_t d = (e + f) % mod;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

static inline idx_t fib_mod(idx_t n, idx_t mod)
{
    idx_t fn, fn1;
    fib_mod_pair(n, mod, &fn, &fn1);
    return fn % mod;
}

/* Main sequence function.  Returns the required m as seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard for safety */

    idx_t m = n + 1;
    for (;;) {
        idx_t mod = m + n;          /* m+n > 0 */
        idx_t fn  = fib_mod(n, mod);
        idx_t fm  = fib_mod(m, mod);
        if ((fm + fn) % mod == 0)
            return (seq_t)m;
        ++m;
    }
}
