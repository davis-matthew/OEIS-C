/*
 * A344530  For any number n with binary expansion Sum_{k = 1..m} 2^e_k (where
 *          0 <= e_1 < ... < e_m), a(n) = Product_{k = 1..m} prime(1+e_k)^k
 *          (where prime(k) denotes the k-th prime number).
 * https://oeis.org/A344530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{k=1..m} prime(1+e_k)^k where the binary expansion of n
 *            is n = ∑_{k=1..m} 2^e_k, with 0 ≤ e_1 < … < e_m
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 94, u128 n <= 478,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This definition is implemented directly without memoisation. */

#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;        /* accumulator, a(0)=1 by definition */
    idx_t e = 0;           /* current bit position (0‑based) */
    idx_t rank = 0;        /* rank of the current set bit (1‑based) */

    while (n != 0) {
        if (n & 1U) {       /* bit e is set */
            ++rank;                      /* first set bit gets rank 1 */
            seq_t prime = (seq_t)get_prime(e + 1U);   /* prime(1+e)   */
            seq_t pow   = big_pow(prime, (seq_t)rank);/* prime^rank   */
            prod *= pow;                  /* multiply into product */
        }
        n >>= 1;           /* move to next bit */
        ++e;
    }
    return prod;          /* for n==0 prod remains 1 */
}
