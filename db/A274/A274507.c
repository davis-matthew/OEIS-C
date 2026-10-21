/*
 * A274507  Primes one more than the sum over a pair of prime numbers that
 *          differ by 8.
 * https://oeis.org/A274507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A274507:
 * primes p such that p = 2*q + 9 where q and q+8 are primes. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"       /* bool is_prime(idx_t x); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n < 1, harmless */

    idx_t count = 0;
    uint64_t q = 3;                 /* start counting odd numbers */

    while (1) {
        /* Check that q is prime */
        if (is_prime(q)) {
            uint64_t q_plus8 = q + 8;
            /* q+8 must also be prime */
            if (is_prime(q_plus8)) {
                seq_t p_val = (seq_t)q * 2 + 9;
                /* p_val must be prime (big integer test) */
                if (is_prime_big(p_val)) {
                    ++count;
                    if (count == n) return p_val;
                }
            }
        }
        /* Move to the next odd candidate */
        if (q == 2) q = 3;
        else        q += 2;
    }
}
