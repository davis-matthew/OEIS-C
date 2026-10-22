/*
 * A325121  Sum of binary digits of the prime indices of n.
 * https://oeis.org/A325121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Σ_{p^e‖n} e · popcount(index(p)) where index(p) is the
 *            position of prime p in the increasing list of primes (index
 *            starts at 1). Popcount is the number of 1‑bits in the binary
 *            representation of the index
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Forward declaration for external linkage */
seq_t a(idx_t n);

/* Simple primality sieve state, rebuilt when needed. */
static uint64_t *primes = NULL;          /* array of primes */
static size_t   prime_cnt = 0;           /* number of primes stored */
static uint32_t *prime_to_idx = NULL;    /* index(p) for p <= limit (1‑based) */
static uint64_t limit = 0;               /* current maximum number sieved */

/* Ensure that primes and index map are known for all numbers up to up_to. */
static void ensure_primes_upto(uint64_t up_to)
{
    if (up_to <= limit) return;

    /* Allocate sieve array */
    uint8_t *sieve = (uint8_t *)calloc(up_to + 1, sizeof(uint8_t));
    if (!sieve) exit(EXIT_FAILURE);       /* incomplete implementation */

    /* Eratosthenes */
    for (uint64_t i = 2; i * i <= up_to; ++i)
        if (!sieve[i])
            for (uint64_t j = i * i; j <= up_to; j += i)
                sieve[j] = 1;

    /* Count primes and allocate storage */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= up_to; ++i)
        if (!sieve[i]) ++cnt;

    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) exit(EXIT_FAILURE);

    uint32_t *new_map = (uint32_t *)calloc(up_to + 1, sizeof(uint32_t));
    if (!new_map) exit(EXIT_FAILURE);

    /* Fill primes array and index map (1‑based indices) */
    size_t pos = 0;
    for (uint64_t i = 2; i <= up_to; ++i) {
        if (!sieve[i]) {
            new_primes[pos] = i;
            new_map[i] = (uint32_t)(pos + 1);
            ++pos;
        }
    }

    /* Replace old data */
    free(primes);
    free(prime_to_idx);
    free(sieve);

    primes = new_primes;
    prime_cnt = cnt;
    prime_to_idx = new_map;
    limit = up_to;
}

/* Count 1-bits in a 32‑bit unsigned integer. */
static inline uint32_t popcount_uint32(uint32_t x)
{
    uint32_t c = 0;
    while (x) { c += (x & 1U); x >>= 1U; }
    return c;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Ensure primes and index map up to n (all factors of n are ≤ n). */
    ensure_primes_upto((uint64_t)n);

    uint64_t tmp = n;
    uint64_t sum = 0;

    /* Factorize n and accumulate contributions. */
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p != 0) continue;

        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }

        uint32_t idx = prime_to_idx[p];
        uint32_t pc = popcount_uint32(idx);
        sum += exp * (uint64_t)pc;
    }

    /* If anything remains, it is a prime factor > sqrt(n). */
    if (tmp > 1) {
        uint64_t p = tmp;
        uint32_t idx = prime_to_idx[p];
        uint32_t pc = popcount_uint32(idx);
        sum += pc;          /* exponent is 1 */
    }

    return (seq_t)sum;
}
