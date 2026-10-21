/*
 * A278244  Least number with the prime signature of the n-th centered square
 *          number (A001844(n)).
 * https://oeis.org/A278244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A001844(n)) A001844(n) = 2*n^2 + 2*n + 1 A046523
 *            gives the least integer having the same prime signature as its
 *            argument
 * Range:     u8 n <= 10, u16 n <= 2258, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Smallest primes used to construct the minimal number for a signature.
   We need enough primes for the maximal number of exponents (≤10 for the
   range of n considered). */
static const uint64_t SMALL_PRIMES[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};

/* Fast exponentiation for a small integer exponent. */
static seq_t pow_small(seq_t base, unsigned exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Compute the least integer that has the same prime signature as
   the 64‑bit integer m. */
static seq_t least_with_signature_from_uint64(uint64_t m) {
    /* Extract exponents of prime factors of m. */
    unsigned exponents[32];
    unsigned excnt = 0;

    if (m <= 1)          /* empty signature → 1 */
        return 1;

    /* factor 2 */
    unsigned e = 0;
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e)
        exponents[excnt++] = e;

    /* factor odd primes */
    uint64_t p = 3ULL;
    while (p * p <= m) {
        e = 0;
        while (m % p == 0) {
            ++e;
            m /= p;
        }
        if (e)
            exponents[excnt++] = e;
        p += 2ULL;
    }
    if (m > 1)        /* remaining prime factor */
        exponents[excnt++] = 1;

    /* sort exponents in descending order (simple insertion sort). */
    for (unsigned i = 1; i < excnt; ++i) {
        unsigned key = exponents[i];
        unsigned j = i;
        while (j > 0 && exponents[j-1] < key) {
            exponents[j] = exponents[j-1];
            --j;
        }
        exponents[j] = key;
    }

    /* build the least number: assign largest exponent to 2, next to 3, etc. */
    seq_t result = 1;
    for (unsigned i = 0; i < excnt; ++i) {
        result *= pow_small((seq_t)SMALL_PRIMES[i], exponents[i]);
    }
    return result;
}

seq_t a(idx_t n) {
    /* Centered square number: 2*n^2 + 2*n + 1 */
    seq_t cs = 2 * (seq_t)n * (seq_t)n + 2 * (seq_t)n + 1;

    /* For the size of n considered, cs fits in 64 bits. */
    uint64_t cs_ll = (uint64_t)cs;
    return least_with_signature_from_uint64(cs_ll);
}
