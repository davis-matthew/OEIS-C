/*
 * A352395  Denominator of Sum_{k=0..n} (-1)^k / (2*k+1).
 * https://oeis.org/A352395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of Sum_{k=0..n} (-1)^k/(2*k+1) = lcm{ 1, 3,
 *            5, …, 2*n+1 }
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* The primes array is big enough for n <= 10000 (2*n+1 <= 20001). */
static const uint64_t PRIMES_LIMIT = 20001;
static bool primes_initialized = false;
static uint64_t primes[2263];   /* primes <= 20001 are 2262 in number */
static size_t prime_count = 0;

/* One‑time sieve to fill 'primes'.  Works for all builds. */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    bool sieve[PRIMES_LIMIT + 1];
    for (size_t i = 0; i <= PRIMES_LIMIT; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (uint64_t i = 2; i * i <= PRIMES_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= PRIMES_LIMIT; j += i)
                sieve[j] = false;
        }
    }

    for (uint64_t i = 2; i <= PRIMES_LIMIT; ++i)
        if (sieve[i])
            primes[prime_count++] = i;

    primes_initialized = true;
}

/* ------------------ public function ------------------ */
seq_t a(idx_t n)
{
    init_primes();

    /* n is uint64_t, we assume n <= 10000; higher values would need
       a larger sieve, which is not required for the tests. */
    uint64_t limit = 2 * n + 1;

    seq_t result = 1;          /* lcm starts at 1 */

    for (size_t i = 0; i < prime_count && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        if (p == 2)          /* skip the even prime; only odd factors needed */
            continue;

        seq_t power = (seq_t)p;
        /* multiply by the largest power of p not exceeding 'limit' */
        while (1) {
            seq_t next = power * (seq_t)p;
            if (next > (seq_t)limit)
                break;
            power = next;
        }

        result *= power;
    }

    return result;
}
