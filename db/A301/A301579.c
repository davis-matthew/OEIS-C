/*
 * A301579  Least nonnegative integer k such that n^2 - 3*2^k can be written
 *          as x^2 + 2*y^2 with x and y integers, or -1 if no such k exists.
 * https://oeis.org/A301579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k >= 0 such that n^2 - 3·2^k can be expressed as
 *            x^2 + 2·y^2 with integers x,y; returns -1 if no such k exists
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto() */

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* Initialise the prime list once.  Primes up to 100 000 are enough
   for all n that may appear in the tests (n^2 < 10^8). */
static void init_primes(void)
{
    if (!primes_inited) {
        build_primes_upto(100000, &primes, &prime_cnt);
        primes_inited = true;
    }
}

/* Return true iff m > 0 and m can be written as x^2 + 2·y^2.  The
   characterization used is that every prime p ≡ 5 or 7 (mod 8)
   must occur to an even exponent in the prime factorisation of m. */
static bool rep_x2plus2y2_uint64(uint64_t m)
{
    if (m == 0)
        return true;      /* 0 = 0^2 + 2·0^2  */
    uint64_t tmp = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp)
            break;
        if (tmp % p == 0) {
            unsigned exp = 0;
            do {
                tmp /= p;
                ++exp;
            } while (tmp % p == 0);
            uint64_t mod8 = p & 7ULL;
            if ((mod8 == 5ULL || mod8 == 7ULL) && (exp & 1U))
                return false;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        uint64_t mod8 = tmp & 7ULL;
        if (mod8 == 5ULL || mod8 == 7ULL)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return -1;   /* undefined for index 0, but safe fallback */

    init_primes();

    uint64_t n64 = (uint64_t)n;
    uint64_t n2  = n64 * n64;      /* n^2 fits in 64 bits for n ≤ 10 000 */

    for (uint64_t k = 0; k < 64; ++k) {   /* k will never reach 64 in practice */
        uint64_t pow2k = 1ULL << k;
        int64_t  t = (int64_t)n2 - (int64_t)3 * (int64_t)pow2k;

        if (t <= 0)
            return -1;                /* t negative thereafter   */

        if (rep_x2plus2y2_uint64((uint64_t)t))
            return (seq_t)k;          /* cast to seq_t; may truncate if seq_t is too small */
    }
    return -1;   /* safety net – should never be reached */
}
