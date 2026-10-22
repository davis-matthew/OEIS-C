/*
 * A345283  a(n) = Sum_{p|n, p prime} p^nextprime(n/p), where nextprime(n) is
 *          the smallest prime > n.
 * https://oeis.org/A345283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^nextprime(n/p)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A345283 for any n >= 1 using basic trial division
   and the provided big_pow and next_prime utilities. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/next_prime.h"

/* Helper: integer sqrt for uint64_t (used in factorisation) */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1 << 32; /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        uint64_t sq = (uint64_t)mid * (uint64_t)mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset starts at 1 */

    /* Collect distinct prime divisors of n */
    uint64_t primes[64];
    size_t pcnt = 0;
    idx_t m = n;                     /* working copy */

    if ((m & 1ULL) == 0) {           /* factor 2 if present */
        primes[pcnt++] = 2;
        while ((m & 1ULL) == 0) m >>= 1;
    }

    for (idx_t d = 3; d <= isqrt_u64(m); d += 2) {
        if (m % d == 0) {
            primes[pcnt++] = d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) primes[pcnt++] = m;

    /* Sum p^nextprime(n/p) over distinct primes p | n */
    seq_t sum = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        idx_t div = n / p;                      /* n/p, integer */
        uint64_t nxt = next_prime(div);         /* next prime > n/p */
        sum += big_pow((seq_t)p, (seq_t)nxt);
    }
    return sum;
}
