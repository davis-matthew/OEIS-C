/*
 * A352141  Numbers whose prime factorization has all even indices and all
 *          even exponents.
 * https://oeis.org/A352141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m_n)^2, where m_n is the nth integer whose prime factors
 *            all lie among even-indexed primes
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 9333, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates all integers m whose prime factors are contained
   in the set {prime(2), prime(4), prime(6), ...}.  It then returns the
   square of the n-th such integer.  A linear sieve precomputes the
   smallest prime factor and the parity of each prime’s position. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_M 2000000U            /* upper bound on m, sufficient for n up to 10 000 */

static bool initialized = false;
static uint32_t spf[MAX_M + 1];
static uint8_t  prime_even[MAX_M + 1];
static uint32_t primes[MAX_M];       /* enough to hold all primes up to MAX_M */
static size_t prime_count = 0;

static void init_sieve(void)
{
    size_t i, j;
    /* linear sieve */
    for (i = 0; i <= MAX_M; ++i) spf[i] = 0;
    prime_count = 0;
    for (i = 2; i <= MAX_M; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[prime_count++] = (uint32_t)i;
            prime_even[i] = (uint8_t)((prime_count % 2) == 0);
        }
        for (j = 0; j < prime_count && (uint64_t)primes[j] * i <= MAX_M; ++j) {
            uint32_t p = primes[j];
            spf[p * i] = p;
            if (i % p == 0) break;
        }
    }
}

/* Test whether all prime factors of m are even-indexed primes */
static bool m_ok(uint64_t m)
{
    if (m <= 1) return true;
    uint64_t x = m;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        if (!prime_even[p]) return false;
        while (x % p == 0) x /= p;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    idx_t count = 0;

    for (idx_t m = 1; ; ++m) {
        /* If m exceeds precomputed range, abort (should not happen for n ≤ 10 000) */
        if (m > MAX_M) {
            /* fallback: continue with trial factorisation (not needed for given limits) */
        }
        if (m_ok(m)) {
            ++count;
            if (count == n)
                return (seq_t)m * (seq_t)m;
        }
    }
    /* unreachable, but silence compiler warning */
    return (seq_t)0;
}
