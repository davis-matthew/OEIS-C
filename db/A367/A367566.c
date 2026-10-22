/*
 * A367566  a(n) is the product of the primes p <= n+1 such that n * k^n ==
 *          +-1 (mod p) for every k that is not a multiple of p.
 * https://oeis.org/A367566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 377, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by checking primes p ≤ n+1: include p if
   n satisfies the congruence conditions derived from
   n·kⁿ ≡ ±1 (mod p); the result is the product of all such primes. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* idx_t and seq_t are provided by the build system. */

static idx_t primes[200000];
static size_t prime_count = 0;
static idx_t primes_last_checked = 1;

/* Grow the list of primes up to the given limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_last_checked)
        return;

    for (idx_t cand = primes_last_checked + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count < sizeof(primes) / sizeof(primes[0]))
                primes[prime_count++] = cand;
        }
    }
    primes_last_checked = limit;
}

seq_t a(idx_t n)
{
    ensure_primes(n + 1);          /* need all primes ≤ n+1 */

    seq_t prod = 1;                /* product accumulator */

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (p > n + 1)
            break;

        if (p == 2) {
            if (n & 1)            /* n odd → 2 divides a(n) */
                prod *= (seq_t)2;
        } else if (p == 3) {
            if (n > 1 && (n % 3) != 0)   /* 3 divides a(n) iff n>1 and n≠0 mod 3 */
                prod *= (seq_t)3;
        } else {                 /* p > 3 */
            uint64_t mp = (uint64_t)p;
            uint64_t mod = mp * (mp - 1) / 2;    /* p*(p-1)/2 */
            uint64_t rem = (uint64_t)n % mod;
            uint64_t val = mp - 1;               /* p-1 */
            if (rem == val || rem == mod - val)  /* n ≡ ±(p-1) (mod mod) */
                prod *= (seq_t)p;
        }
    }
    return prod;
}
