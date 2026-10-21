/*
 * A247940  Least integer m > n such that m + n divides L(m) + L(n), where
 *          L(k) refers to the Lucas number A000032(k).
 * https://oeis.org/A247940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 3205, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = least m > n such that (m + n) divides L(m) + L(n), 
   where L(k) is the Lucas number. */

#include <stdint.h>

/* Fast doubling: compute F(n) and F(n+1) modulo mod. */
static void fib_pair(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1 % mod;
        return;
    }

    seq_t a, b;                   /* a = F(k), b = F(k+1) */
    fib_pair(n >> 1, mod, &a, &b);

    seq_t two_b = (b * 2) % mod;
    seq_t c = (a * ((two_b + mod) % mod - a + mod)) % mod;    /* F(2k) */
    seq_t d = ((a * a) % mod + (b * b) % mod) % mod;           /* F(2k+1) */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* Return Lucas(n) modulo mod.  L(n)=2*F(n+1)-F(n)  (mod mod). */
static seq_t lucas_mod(idx_t n, seq_t mod)
{
    seq_t fn, fn1;
    fib_pair(n, mod, &fn, &fn1);
    seq_t two_fn1 = (seq_t)2 * fn1 % mod;
    seq_t val = (two_fn1 + mod - fn) % mod;   /* adjust for possible negative */
    return val;
}

seq_t a(idx_t n)
{
    /* n >= 1 as per specification */
    for (idx_t m = n + 1; ; ++m) {
        seq_t mod = (seq_t)(m + n);          /* integer > 0 */
        seq_t Ln = lucas_mod(n, mod);
        seq_t Lm = lucas_mod(m, mod);
        seq_t sum_mod = (Lm + Ln) % mod;
        if (sum_mod == 0)
            return (seq_t)m;
    }
}
