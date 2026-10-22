/*
 * A393612  a(n) = least positive integer k such that prime(n) + 2^k + 2 is
 *          prime, or -1 if no such prime exists.
 * https://oeis.org/A393612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least positive integer k such that prime(n)+2^k+2 is
 *            prime, or -1 if no such k exists
 * Range:     i8 n <= 29, i16 n <= 56, i32 n <= 56, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k) */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; return -1 for n == 0. */
    if (n == 0)
        return (seq_t)-1;

    /* Obtain the n‑th prime (1‑based). */
    seq_t p = (seq_t)get_prime(n);

    /* Search for the first k ≥ 1 such that p + 2^k + 2 is prime. */
    const idx_t MAX_K = 128;      /* safe upper bound for the intended limits */
    seq_t pow2 = (seq_t)2;        /* 2^1 */

    for (idx_t k = 1; k <= MAX_K; ++k)
    {
        seq_t candidate = p + pow2 + (seq_t)2;
        if (is_prime_big(candidate))
            return (seq_t)k;
        pow2 = pow2 * (seq_t)2;   /* next power of two */
    }

    /* No k found within the bound → return -1. */
    return (seq_t)-1;
}
