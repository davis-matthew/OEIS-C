/*
 * A368168  The number of unitary divisors of n that are cubefull
 *          exponentially odd numbers (A335988).
 * https://oeis.org/A368168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^k where k = #{ prime p | exponent e_p(n) is odd and ≥ 3
 *            }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative implementation: for n = ∏ p^e, a(n) = ∏ f(p^e) with f(p^e)=2 if e odd ≥3, else 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t t = (uint64_t)n;          /* work in native 64‑bit integer */
    unsigned cnt = 0;                  /* number of primes with odd exponent ≥3 */

    /* factor out 2 */
    unsigned exp = 0;
    while ((t & 1ULL) == 0) {           /* t % 2 == 0 */
        t >>= 1;                       /* t /= 2 */
        exp++;
    }
    if (exp >= 3 && (exp & 1U)) cnt++;

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned ex = 0;
            while (t % p == 0) {
                t /= p;
                ex++;
            }
            if (ex >= 3 && (ex & 1U)) cnt++;
        }
    }

    /* remaining prime factor (if any) has exponent 1, contributes nothing */
    /* compute 2^cnt */
    seq_t res = 1;
    while (cnt--) res <<= 1;
    return res;
}
