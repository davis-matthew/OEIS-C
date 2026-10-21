/*
 * A262083  Smallest possible prime factor of 10^k+n for any k.
 * https://oeis.org/A262083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A262083:
 * a(n) = smallest prime dividing 10^k + n for some k ≥ 1
 * (with k = 1, 2, …).  For even n, a(n) = 2; otherwise we test primes
 * > 5 in ascending order until the first that satisfies 10^k ≡ -n (mod p)
 * for some k ≥ 1.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_PRIME_LIMIT 200000          /* enough for n ≤ 10000 */

static uint32_t primes[MAX_PRIME_LIMIT];
static size_t   prime_cnt = 0;
static bool     primes_inited = false;

/* Build a list of primes up to MAX_PRIME_LIMIT using a simple sieve. */
static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    static bool sieve[MAX_PRIME_LIMIT + 1] = {false};
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i <= MAX_PRIME_LIMIT) {
                for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    sieve[j] = true;
            }
        }
    }
}

seq_t a(idx_t n)
{
    init_primes();

    /* Even n: 2 divides all 10^k + n for k ≥ 1. */
    if ((n & 1ULL) == 0ULL) return (seq_t)2;

    /* For odd n handle p = 3 and p = 5 specially. */
    uint64_t n64 = (uint64_t)n;
    if (n64 % 3ULL == 2ULL) return (seq_t)3;
    if (n64 % 5ULL == 0ULL) return (seq_t)5;

    /* Search the first prime > 5 that satisfies 10^k ≡ -n (mod p). */
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint32_t p = primes[idx];
        if (p <= 5) continue;            /* already handled 2,3,5 */

        /* Compute target = (-n) mod p = (p - n % p) % p. */
        uint32_t nmod = (uint32_t)(n64 % p);
        uint32_t target = (p - nmod) % p;
        if (target == 0) continue;        /* impossible for p ≠ 2,5 */

        uint32_t curr = 10 % p;
        if (curr == target) return (seq_t)p;
        for (size_t i = 1; i < p; ++i) {   /* at most p‑1 steps */
            curr = (uint64_t)curr * 10 % p;
            if (curr == target) return (seq_t)p;
        }
    }

    /* Theoretically unreachable for n ≤ 10000 with this limit. */
    return (seq_t)0;
}
