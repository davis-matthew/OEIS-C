/*
 * A350070  a(n) = 1 if the maximal digit in the primorial base expansion of n
 *          is greater than or equal to the maximal exponent in the prime
 *          factorization of n, otherwise 0.
 * https://oeis.org/A350070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the maximal digit in the primorial base expansion of
 *            n is greater than or equal to the maximal exponent in the prime
 *            factorization of n; otherwise 0
 * Range:     u8 n <= 8793, u16 n <= 8793, u32 n <= 8793, u64 n <= 8793,
 *            u128 n <= 8793, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* First 16 primes are enough for n < 2^64 (primorial grows very fast). */
static const idx_t PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53
};
#define NUM_PRIMES (sizeof(PRIMES)/sizeof(PRIMES[0]))

seq_t a(idx_t n)
{
    /* 1. maximal digit in primorial base expansion */
    idx_t q = n;
    seq_t max_digit = 0;
    for (size_t i = 0; i < NUM_PRIMES && q > 0; ++i) {
        idx_t p = PRIMES[i];
        seq_t d = (seq_t)(q % p);   /* digit */
        if (d > max_digit) max_digit = d;
        q /= p;                     /* move to next radix position */
    }

    /* 2. maximal exponent in prime factorization */
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t cnt = 0;
    factor_u64(n, primes_buf, exps_buf, &cnt);

    seq_t max_exp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps_buf[i];
        if (e > max_exp) max_exp = e;
    }

    /* 3. compare and return 1 or 0 */
    return (max_digit >= max_exp) ? (seq_t)1 : (seq_t)0;
}
