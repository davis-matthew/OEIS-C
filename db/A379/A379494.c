/*
 * A379494  a(n) = 2*A276086(n) - A276086(A001065(n)), where A276086 is the
 *          primorial base exp-function, and A001065 is the sum of proper
 *          divisors of n.
 * https://oeis.org/A379494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*A276086(n) - A276086(A001065(n))
 * Range:     i8 n <= 10, i16 n <= 56, i32 n <= 208, i64 n <= 2059,
 *            i128 n <= 8316, big any n
 * Helpers:   get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The primorial base exp-function A276086 is implemented below: */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"

static seq_t primorial_exp(seq_t m)
{
    /* Compute A276086(m): product of prime^digit where digits are the mixed-radix
       representation of m with bases equal to successive primes. */
    if (m == 0) return 1;          /* A276086(0) is defined as 1 */
    seq_t result = 1;
    seq_t weight = 1;              /* product of primes used as weights */
    idx_t k = 1;                   /* 1‑based index of the current prime */

    while (weight <= m)
    {
        uint64_t prime = get_prime(k);           /* current prime p_k */
        seq_t digit = (m / weight) % prime;      /* a_k  (0 <= a_k < prime) */

        /* multiply result by prime^digit */
        seq_t mult = 1;
        for (seq_t i = 0; i < digit; ++i)
            mult *= prime;
        result *= mult;

        /* advance weight: weight *= p_k for the next round */
        weight *= prime;
        ++k;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* A276086(n) */
    seq_t f_n = primorial_exp((seq_t)n);

    /* A001065(n) = sum of proper divisors = sigma_of(n) - n */
    seq_t proper_sum = sigma_of(n) - (seq_t)n;

    /* A276086(A001065(n)) */
    seq_t f_sum = primorial_exp(proper_sum);

    /* 2*A276086(n) - A276086(A001065(n)) */
    return f_n + f_n - f_sum;
}
