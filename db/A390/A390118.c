/*
 * A390118  Least positive integer m such that m*n + 1 = p^2 for some prime
 *          number p.
 * https://oeis.org/A390118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer m such that n*m + 1 = p² for some
 *            prime p
 * Range:     u8 n <= 16, u16 n <= 642, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool primes_ready = false;
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_ready)
        return;

    const uint64_t LIMIT = 2000000ULL;  /* sufficient for n up to ~10 000 (p < ~1 100 000) */
    uint8_t *sieve = (uint8_t *)malloc((size_t)(LIMIT + 1));
    if (!sieve)
        exit(EXIT_FAILURE);
    memset(sieve, 1, (size_t)(LIMIT + 1));
    sieve[0] = sieve[1] = 0;

    for (uint64_t i = 2; i * i <= LIMIT; ++i)
    {
        if (sieve[i])
        {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (sieve[i])
            ++cnt;
    prime_cnt = cnt;

    primes = (uint32_t *)malloc(sizeof(uint32_t) * prime_cnt);
    if (!primes)
        exit(EXIT_FAILURE);

    size_t pos = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
    {
        if (sieve[i])
            primes[pos++] = (uint32_t)i;
    }

    free(sieve);
    primes_ready = true;
}

static bool is_prime_uint64(uint64_t v)
{
    if (v < 2)
        return false;
    if (v == 2 || v == 3)
        return true;
    if ((v & 1) == 0)
        return false;
    for (uint64_t d = 3; d * d <= v; d += 2)
        if (v % d == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    init_primes();
    const uint64_t n64 = (uint64_t)n;

    /* search primes up to the pre‑computed limit */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = (uint64_t)primes[i];
        uint64_t sq = p * p;       /* fits in 64 bits for p ≤ 2 000 000 */
        if ((sq - 1ULL) % n64 == 0ULL)
        {
            uint64_t m = (sq - 1ULL) / n64;
            return (seq_t)m;
        }
    }

    /* fallback: continue generating primes beyond the sieve limit */
    uint64_t p = 2;
    while (true)
    {
        if (is_prime_uint64(p))
        {
            uint64_t sq = p * p;
            if ((sq - 1ULL) % n64 == 0ULL)
            {
                uint64_t m = (sq - 1ULL) / n64;
                return (seq_t)m;
            }
        }
        ++p;
    }

    /* unreachable, sequence is well‑defined for every n >= 1 */
    return (seq_t)0;
}
