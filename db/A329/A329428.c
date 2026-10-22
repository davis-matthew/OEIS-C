/*
 * A329428  Starting from n: as long as the decimal representation starts with
 *          a prime number, replace the largest such prefix with the index of
 *          the corresponding prime number; a(n) corresponds to the final
 *          value.
 * https://oeis.org/A329428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by repeatedly removing a prime prefix and
 *            replacing it with its rank
 * Range:     u8 n <= 398, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/compute_pi.h"

static uint32_t *prime_list = NULL;      /* list of primes */
static size_t   prime_cnt  = 0;           /* number of primes */
static uint32_t prime_limit = 0;          /* sieve upper bound */

/* Build the prime sieve up to `limit` (inclusive).  The array is
   increased only if a larger `limit` is requested. */
static void ensure_primes_upto(uint32_t limit)
{
    if (limit <= prime_limit) return;

    uint32_t n = limit + 1;
    uint8_t *comp = (uint8_t *)calloc(n, 1);   /* 0 = prime, 1 = composite */
    if (!comp) exit(EXIT_FAILURE);

    for (uint32_t i = 2; (uint64_t)i * i < n; ++i)
        if (!comp[i])
            for (uint32_t j = i * i; j < n; j += i)
                comp[j] = 1;

    size_t total = 0;
    for (uint32_t i = 2; i < n; ++i)
        if (!comp[i]) ++total;

    uint32_t *new_primes = (uint32_t *)realloc(prime_list,
                                               total * sizeof(uint32_t));
    if (!new_primes) exit(EXIT_FAILURE);
    prime_list = new_primes;

    size_t pos = 0;
    for (uint32_t i = 2; i < n; ++i)
        if (!comp[i]) prime_list[pos++] = i;

    prime_cnt  = total;
    prime_limit = limit;
    free(comp);
}

static inline uint32_t prime_index(uint32_t x)
{
    /* Binary search for x in prime_list (assumes primes built up to > x). */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == x) return (uint32_t)(mid + 1);
        if (prime_list[mid] < x) lo = mid + 1;
        else                     hi = mid;
    }
    return 0;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;

    /* Ensure primes up to the current value (needed for prefixes). */
    if (cur <= (seq_t)UINT32_MAX)
        ensure_primes_upto((uint32_t)cur);

    while (1) {
        /* Count decimal digits of cur. */
        size_t digits = 0;
        seq_t t = cur;
        do { t /= (seq_t)10; ++digits; } while (t);

        seq_t pow10k = (seq_t)1;   /* 10^k, k grows from 0 upwards. */
        bool changed = false;

        for (size_t k = 0; k < digits; ++k) {
            seq_t prefix = cur / pow10k;
            if (prefix <= (seq_t)UINT32_MAX) {
                uint32_t p32 = (uint32_t)prefix;
                /* test primality and find its index. */
                if (is_prime_big(prefix) && prime_index(p32) != 0) {
                    uint32_t idx = prime_index(p32);
                    seq_t remainder = cur % pow10k;
                    cur = (seq_t)idx * pow10k + remainder;
                    changed = true;
                    break;               /* restart from new value */
                }
            }
            pow10k *= (seq_t)10;
        }

        if (!changed) break;          /* no prime prefix found */
        /* make sure we can test further prefixes if cur grew larger */
        if (cur <= (seq_t)UINT32_MAX)
            ensure_primes_upto((uint32_t)cur);
    }

    return cur;
}
