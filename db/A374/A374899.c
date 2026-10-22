/*
 * A374899  a(n) is the least semiprime that starts with n.
 * https://oeis.org/A374899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the least semiprime that starts with the decimal digits
 *            of n
 * Range:     u8 n <= 6, u16 n <= 655, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, floor_sqrt, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes.h"   /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/floor_sqrt.h"     /* seq_t floor_sqrt(seq_t x); */

/* prime sieve limit – chosen to cover the square roots of all numbers searched */
static const idx_t PRIME_LIMIT = 4000000ULL;

/* static storage for the prime list */
static idx_t *primes = NULL;
static size_t prime_cnt = 0;

/* helper: test whether a number is a semiprime (product of exactly two primes, possibly equal) */
static bool is_semiprime(seq_t m) {
    if (m < 4) /* 4 is the smallest semiprime */
        return false;

    /* get integer square root of m */
    seq_t sqrt_m_seq = floor_sqrt(m);
    idx_t sqrt_m = (idx_t)sqrt_m_seq;

    /* search for a small factor */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];

        /* stop if p > sqrt(m) */
        if ((seq_t)p * (seq_t)p > m)
            break;

        if (m % (seq_t)p == 0) {
            seq_t q = m / (seq_t)p;           /* the co‑factor */
            if (q <= 1)                      /* should never happen */
                return false;
            /* q must be prime for m to be a semiprime */
            return is_prime_big(q);
        }
    }

    /* no divisor found ⇒ m is prime */
    return false;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* initialise the prime list on first call */
    if (!primes) {
        build_primes(PRIME_LIMIT, &primes, &prime_cnt);
    }

    seq_t n_seq = (seq_t)n;

    for (idx_t k = 0; ; ++k) {
        seq_t powk = pow10(k);
        seq_t lower = n_seq * powk;
        seq_t upper = ((seq_t)(n + 1) * powk) - 1;

        /* iterate over all numbers that start with the decimal digits of n */
        for (seq_t m = lower; ; ++m) {
            if (m > upper)            /* reached the end of this block */
                break;
            if (is_semiprime(m))
                return m;            /* found the least semiprime with the desired prefix */
        }
    }
}
