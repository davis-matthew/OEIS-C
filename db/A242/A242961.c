/*
 * A242961  The smallest prime p > prime(n) such that p mod prime(n) == - 1.
 * https://oeis.org/A242961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 261, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: smallest prime > prime(n) congruent to -1 modulo prime(n). */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* a(1) = 3, a(2) = 5, a(3) = 19, ... */
seq_t a(idx_t n)
{
    uint32_t q_uint = get_prime(n);          /* prime(n) */
    seq_t q = (seq_t)q_uint;                 /* cast to seq_t */

    idx_t k = 2;                             /* start search from 2*prime(n)-1 */
    while (1) {
        seq_t p = ((seq_t)k) * q - (seq_t)1; /* candidate = k*q - 1 */
        if (is_prime_big(p)) {
            return p;                       /* found smallest such prime */
        }
        ++k;                                /* try next k */
    }
}
