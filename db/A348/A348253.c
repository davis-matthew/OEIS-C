/*
 * A348253  Indices of records in A348246.
 * https://oeis.org/A348253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348253: Indices of records in A348246
   a(n) = n                 for 1 ≤ n ≤ 5
   a(6) = 7
   a(7) = 10
   a(n) = prime_{n-3}       for n ≥ 8   (1‑based indexing of primes) */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n <= 5) return (seq_t)n;
    if (n == 6) return (seq_t)7;
    if (n == 7) return (seq_t)10;
    /* n ≥ 8 */
    return (seq_t)get_prime(n - 3);
}
