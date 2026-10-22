/*
 * A304917  a(n) = prime(n)^n - primorial(n - 1).
 * https://oeis.org/A304917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^n - primorial(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

/* Compute the n‑th term of the sequence.
   a(1) = 1, a(2) = 7, …  */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for the offset 1, but safe */
        return 0;

    /* prime(n) as seq_t */
    seq_t prime_n = (seq_t)get_prime(n);

    /* prime(n)^n */
    seq_t power = seq_pow(prime_n, n);

    /* primorial(n-1) = product of the first (n-1) primes */
    seq_t primorial = 1;
    for (idx_t i = 1; i < n; ++i)
        primorial *= (seq_t)get_prime(i);

    return power - primorial;
}
