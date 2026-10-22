/*
 * A380906  Primes without {3, 5} as digits.
 * https://oeis.org/A380906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1766, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth prime number that does not contain the decimal
 *              digits 3 or 5.
 * Implementation: incremental prime generation with trial division
 *                 using the list of all primes found so far.
 *                 A static array of qualifying primes is stored for
 *                 subsequent calls.  Digit checking is performed
 *                 only for primes.
 */
#include <stdlib.h>
#include <stdbool.h>

/* static storage for all primes generated so far (for trial division) */
static uint64_t *allPrimes = NULL;
static size_t  allCap   = 0;
static size_t  allLen   = 0;

/* static storage for qualifying primes (primes without 3 or 5) */
static uint64_t *qualPrimes = NULL;
static size_t  qualCap   = 0;
static size_t  qualLen   = 0;

/* next candidate to test for primality */
static uint64_t nextCandidate = 2;

/* enlarge an array of uint64_t */
static void enlarge(uint64_t **arr, size_t *cap) {
    if (*cap == 0) {
        *cap = 16;
        *arr = (uint64_t *)malloc(*cap * sizeof(uint64_t));
    } else {
        *cap <<= 1;
        *arr = (uint64_t *)realloc(*arr, *cap * sizeof(uint64_t));
    }
}

/* return the next prime > current nextCandidate, updating prime list */
static uint64_t next_prime(void) {
    for (;;) {
        uint64_t cand = nextCandidate;
        bool isP = true;
        for (size_t i = 0; i < allLen; ++i) {
            uint64_t p = allPrimes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isP = false; break; }
        }
        nextCandidate = cand + 1;
        if (isP) {
            if (allLen == allCap) enlarge(&allPrimes, &allCap);
            allPrimes[allLen++] = cand;
            return cand;
        }
    }
}

/* return the next qualifying prime (without digits 3 or 5) */
static uint64_t next_qual_prime(void) {
    for (;;) {
        uint64_t p = next_prime();
        uint64_t x = p;
        bool ok = true;
        while (x > 0) {
            uint64_t d = x % 10;
            if (d == 3 || d == 5) { ok = false; break; }
            x /= 10;
        }
        if (ok) return p;
    }
}

/* public function required by the build */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, but safe fallback */

    /* generate qualifying primes until we have at least n */
    while (qualLen < n) {
        uint64_t p = next_qual_prime();
        if (qualLen == qualCap) enlarge(&qualPrimes, &qualCap);
        qualPrimes[qualLen++] = p;
    }
    return (seq_t)qualPrimes[n - 1];
}
