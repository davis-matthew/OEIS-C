/*
 * A344219  Number of cyclic subgroups of the group (C_n)^5, where C_n is the
 *          cyclic group of order n.
 * https://oeis.org/A344219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e||n} [ 1 + ((p^5-1)/(p-1)) *
 *            ((p^(4e)-1)/(p^4-1)) ] (multiplicative; a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 197, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided factor_u64 and pow_seq utilities. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

/* Computes a(idx_t n).  Assumes n >= 1.  */
seq_t a(idx_t n)
{
    /* Handle the multiplicative identity. */
    if (n == 1)  /* a(1) = 1 */
        return (seq_t)1;

    /* Factor n into primes.  The maximum number of distinct primes for
       n <= 10 000 is small ( < 14 ), so an array of size 20 is more than enough. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        unsigned e = (unsigned)exps[i];
        seq_t p_seq = (seq_t)p;

        /* t1 = 1 + p + p^2 + p^3 + p^4  (i.e. (p^5-1)/(p-1)). */
        seq_t t1 = (seq_t)1;
        seq_t pow = (seq_t)1;
        for (int k = 0; k < 4; ++k) {  /* multiply by p k+1 times and add to t1 */
            pow *= p_seq;
            t1 += pow;
        }

        /* denom2 = p^4 - 1. */
        seq_t denom2 = pow_seq(p_seq, 4) - (seq_t)1;

        /* numerator2 = p^(4e) - 1. */
        seq_t numerator2 = pow_seq(p_seq, 4 * e) - (seq_t)1;

        /* t2 = numerator2 / denom2.  Exact division by construction. */
        seq_t t2 = numerator2 / denom2;

        /* a(p^e) = 1 + t1 * t2. */
        seq_t a_pe = t1 * t2 + (seq_t)1;

        /* Multiply into the overall result. */
        result *= a_pe;
    }

    return result;
}
