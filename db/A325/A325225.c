/*
 * A325225  Lesser of the number of prime factors of n counted with
 *          multiplicity and the maximum prime index of n.
 * https://oeis.org/A325225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = min(A001222(n), A061395(n))
 * Range:     u8 n <= 9060, u16 n <= 9060, u32 n <= 9060, u64 n <= 9060,
 *            u128 n <= 9060, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = lesser of the number of prime factors of n with multiplicity
 * and the maximum prime index of n.  The function is O(√n) per call and
 * precomputes the primes up to a fixed limit the first time it is used. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum prime to pre‑compute (enough for n up to 10000). */
#define PRIME_LIMIT 200000
#define MAX_PRIMES  25000

static uint32_t primes[MAX_PRIMES];
static size_t  prime_cnt = 0;
static bool    primes_ready = false;
static bool    is_comp[PRIME_LIMIT + 1];

/* Pre‑compute the primes with a simple sieve. */
static void init_primes(void)
{
    if (primes_ready)
        return;

    for (uint32_t i = 0; i <= PRIME_LIMIT; ++i)
        is_comp[i] = false;

    prime_cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt < MAX_PRIMES)
                primes[prime_cnt++] = i;
            if ((uint64_t)i * (uint64_t)i <= PRIME_LIMIT) {
                for (uint64_t j = (uint64_t)i * (uint64_t)i;
                     j <= PRIME_LIMIT; j += i)
                    is_comp[(uint32_t)j] = true;
            }
        }
    }
    primes_ready = true;
}

/* Return the index (1‑based) of prime p in the primes array. */
static seq_t prime_index(uint64_t p)
{
    /* binary search – primes are sorted */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] == p)
            return (seq_t)(mid + 1);
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;  /* should not happen for valid inputs */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!primes_ready)
        init_primes();

    if (n < 2)
        return 0;              /* a(1) = 0 by definition */

    uint64_t temp = (uint64_t)n;
    seq_t bigOmega = 0;
    seq_t maxIdx   = 0;

    /* trial division by pre‑computed primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > temp)
            break;
        if (temp % p == 0) {
            seq_t idx = (seq_t)(i + 1);   /* prime index of p */
            if (idx > maxIdx) maxIdx = idx;
            while (temp % p == 0) {
                ++bigOmega;
                temp /= p;
            }
        }
    }

    /* if a prime factor remains > sqrt(original n) */
    if (temp > 1) {
        ++bigOmega;
        seq_t idx = prime_index(temp);
        if (idx > maxIdx) maxIdx = idx;
    }

    return (bigOmega < maxIdx) ? bigOmega : maxIdx;
}
