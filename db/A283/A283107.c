/*
 * A283107  Numbers k such that tau(4*(k - 1)) is prime.
 * https://oeis.org/A283107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(p_{n+1} - 3) + 1, where p_m is the m‑th prime
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This equals the n‑th integer k for which tau(4*(k-1)) is prime. */

#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; below offset is undefined */
        return (seq_t)0;

    /* Retrieve the (n+1)-th prime (p1 = 2, p2 = 3, …). */
    uint32_t prime = get_prime(n + 1);

    /* Exponent is prime - 3 (since m = p - 3). */
    seq_t exp = (seq_t)(prime - 3u);

    /* Compute 2^exp and add 1. */
    seq_t val = big_pow((seq_t)2, exp);
    val += (seq_t)1;
    return val;
}
