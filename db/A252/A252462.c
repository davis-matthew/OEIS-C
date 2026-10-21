/*
 * A252462  Shift one instance of the largest prime one step towards smaller
 *          primes: a(1) = 1, a(2^n) = 2^(n-1), and for other numbers: a(n) =
 *          (n / prime(g)) * prime(g-1), where g = A061395(n), index of the
 *          greatest prime dividing n.
 * https://oeis.org/A252462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2^k) = 2^(k-1); For n > 1 that is not a power of 2:
 *            let p = largest prime divisor of n, let q = previous prime < p,
 *            then a(n) = (n / p) * q. This equivalent to a(n) = (n /
 *            prime(g)) * prime(g-1) where g = index of the greatest prime
 *            dividing n
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/largest_prime_divisor.h"

static uint64_t prev_prime_of(uint64_t p)
{
    if (p <= 2) return 1;           /* not used for power‑of‑2 case */
    uint64_t cand = p - 1;
    if ((cand & 1) == 0)            /* skip even numbers */
        cand--;
    while (cand >= 3)
    {
        if (is_prime_small((seq_t)cand))
            return cand;
        cand -= 2;
    }
    return 2;                        /* fallback, should not happen */
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* handle powers of two: n = 2^k  (k >= 1) */
    if ((n & (n - 1)) == 0)
    {
        idx_t exp = 0;               /* k such that n = 2^k */
        idx_t tmp = n;
        while (tmp > 1)
        {
            tmp >>= 1;
            ++exp;
        }
        seq_t res = (seq_t)1;
        res <<= (exp - 1);           /* 2^(k-1) */
        return res;
    }

    /* generic case: shift one occurrence of the largest prime downwards */
    uint64_t largest_p = largest_prime_divisor((uint64_t)n);
    uint64_t prev_p    = prev_prime_of(largest_p);
    seq_t res = ((seq_t)n / (seq_t)largest_p) * (seq_t)prev_p;
    return res;
}
