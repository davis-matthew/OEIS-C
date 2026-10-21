/*
 * A286480  Compound filter (prime signature of n & prime signature of
 *          n+d(n)): a(n) = P(A046523(n), A286479(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function and d(n) is number of
 *          divisors of n (A000005).
 * https://oeis.org/A286480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((A046523(n)+A286479(n))^2) - A046523(n) -
 *            3*A286479(n))
 * Range:     u8 n <= 7, u16 n <= 347, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046523(n)  = prime signature of n (product 2^e1 * 3^e2 * 5^e3 ... with e1≥e2≥…)  */
/* A286479(n) = prime signature of n + d(n)  (d(n) = number of divisors of n)  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility functions supplied by the harness */
#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"

/* Small primes used for the canonical prime signature */
static const uint64_t MIN_PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};

/* Return the “prime signature” of m - product of MIN_PRIMES^exponents
 * sorted in non‑increasing order of exponents */
static seq_t prime_signature(uint64_t m)
{
    if (m == 1)
        return (seq_t)1;            /* signature of 1 is defined as 1 */

    /* Factor m: primes[], exps[] contain the factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* Sort exponents in descending order (simple insertion sort) */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t key = exps[i];
        size_t j = i;
        while (j > 0 && exps[j - 1] < key) {
            exps[j] = exps[j - 1];
            --j;
        }
        exps[j] = key;
    }

    /* Build the canonical product */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)MIN_PRIMES[i];
        seq_t power = (seq_t)1;
        for (uint64_t k = 0; k < exps[i]; ++k)
            power = power * base;
        result = result * power;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Number of divisors of n */
    uint64_t d = divisor_count_u64((uint64_t)n);

    /* Compute n + d(n) (safe for all indices in the tests) */
    uint64_t n_plus_d = (uint64_t)((unsigned __int128)n + d);

    /* Prime signatures */
    seq_t sig_n      = prime_signature((uint64_t)n);
    seq_t sig_n_plus = prime_signature(n_plus_d);

    /* PAIRING: (1/2)*(2 + (sig_n+sig_n_plus)^2 - sig_n - 3*sig_n_plus) */
    seq_t sum   = sig_n + sig_n_plus;
    seq_t square = sum * sum;
    seq_t numerator = (seq_t)2 + square - sig_n - (seq_t)3 * sig_n_plus;
    seq_t value = numerator / (seq_t)2;   /* Division is exact */

    return value;
}
