/*
 * A304903  Least prime p such that 2*n^2 - p is prime.
 * https://oeis.org/A304903
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that 2*n^2 - p is prime. Offset: 2
 *            (a(2) is the first term)
 * Range:     u8 n <= 11, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is assumed ≥ 2; behaviour for smaller n is unspecified. */
    seq_t nn = n;                     /* cast to seq_t */
    seq_t two_n2 = ((seq_t)2 * nn) * nn;  /* compute 2*n^2 in seq_t */

    /* Search for the smallest prime p with 2*n^2 - p also prime. */
    for (idx_t p = 2;; p += (p == 2 ? 1 : 2)) {
        seq_t p_seq = p;              /* cast to seq_t */
        seq_t q_seq = two_n2 - p_seq; /* 2*n^2 - p */

        /* test primality of p and q. */
        if (is_prime_big(p_seq) && is_prime_big(q_seq))
            return p_seq;            /* p is the desired value */
    }
}
