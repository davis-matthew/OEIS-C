/*
 * A267697  Numbers with 7 odd divisors.
 * https://oeis.org/A267697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th smallest integer of the form p^6 * 2^k where p
 *            is an odd prime and k ≥ 0. The implementation uses binary search
 *            over the value of a(n) and counts how many such numbers are ≤ a
 *            candidate. Only a small sieve of primes up to 20000 is needed,
 *            because for n ≤ 10000 the largest prime u...
 * Range:     u8 none, u16 n <= 10, u32 n <= 107, u64 n <= 2718,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* --------- small prime sieve ------------------------------------- */
#define PRIME_SIEVE_LIMIT 20000          /* covers all needed primes */
#define MAX_PRIMES 3000

static uint32_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;

static void init_prime_sieve(void)
{
    bool sieve[PRIME_SIEVE_LIMIT + 1];
    for (int i = 0; i <= PRIME_SIEVE_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[j] = false;

    prime_cnt = 0;
    for (int i = 3; i <= PRIME_SIEVE_LIMIT; i += 2)
        if (sieve[i]) primes[prime_cnt++] = (uint32_t)i;
}

/* --------- count numbers <= x ---------------------------------- */
static size_t count_leq(seq_t x)
{
    size_t total = 0;
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint32_t p = primes[idx];
        /* compute p^6 but stop if it exceeds x */
        seq_t base = 1;
        for (int e = 0; e < 6; ++e) {
            if (base > x / (seq_t)p) { base = x + 1; break; }
            base *= (seq_t)p;
        }
        if (base > x) break;           /* further primes give larger bases */

        seq_t t = x / base;
        /* floor_log2(t) */
        unsigned long long lg = 0;
        while (t > 1) { t >>= 1; ++lg; }
        total += lg + 1;               /* k = 0 … floor_log2(t) */
    }
    return total;
}

/* --------- main sequence function -------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_prime_sieve();
        initialized = true;
    }

    if (n == 0) return 0;            /* undefined for 0, return 0 */

    seq_t low  = 0;
    seq_t high = 1;
    /* grow high until it contains at least n terms */
    while ((size_t)count_leq(high) < (size_t)n) {
        seq_t new_high = high << 1;
        if (new_high <= high) {       /* overflow protection */
            high = ~((seq_t)0);
            break;
        }
        high = new_high;
    }

    /* binary search */
    while (low + 1 < high) {
        seq_t mid = low + (high - low) / 2;
        if ((size_t)count_leq(mid) >= (size_t)n)
            high = mid;
        else
            low = mid;
    }
    return high;
}
