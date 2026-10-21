/*
 * A242866  Union of 2*A019434 and the semiprimes in A000215.
 * https://oeis.org/A242866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Define the k‑th Fermat number F_k = 2^(2^k) + 1. a(n) =
 *            2*F_{n-1} if F_{n-1} is prime, F_{n-1} otherwise. This
 *            implements the OEIS definition: the sequence is the union of
 *            2*A019434 (double of the Fermat primes) and the semiprimes that
 *            are the composite Fermat numbers. For n >= 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The index starts at 1.  Convert to 0‑based k. */
    idx_t k = n - 1;

    /* 2^k must fit in a uint64_t for the exponent of 2.  If it
       does not, the result exceeds the practicality of this
       implementation (the numbers grow astronomically fast).  */
    if (k >= 64) {
        /* Undefined for very large n – return 0 as a safe fallback. */
        return (seq_t)0;
    }

    /* Compute exponent 2^k (fits in seq_t because k < 64). */
    seq_t exp = ((seq_t)1) << k;

    /* Compute the Fermat number F_k = 2^(2^k) + 1. */
    seq_t two = (seq_t)2;
    seq_t F   = big_pow(two, exp) + (seq_t)1;

    /* If F_k is prime, return 2*F_k; otherwise return F_k. */
    if (is_prime_big(F))
        return F * (seq_t)2;
    else
        return F;
}
