/*
 * A371201  a(n) = Sum_{k=prime(n)..prime(n+1)-1} k, with a(0) = 1.
 * https://oeis.org/A371201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1, a(n) =
 *            (prime(n+1)-prime(n))*(prime(n+1)+prime(n)-1)/2
 * Range:     u8 n <= 14, u16 n <= 428, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility get_prime(idx_t k) which returns the k‑th prime. */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* get the n‑th and (n+1)‑th primes */
    seq_t p = (seq_t)get_prime(n);      /* prime(n)   */
    seq_t q = (seq_t)get_prime(n + 1);  /* prime(n+1) */

    seq_t diff = q - p;                 /* prime(n+1) - prime(n) */
    seq_t sum  = q + p - 1;             /* prime(n+1) + prime(n) - 1 */

    return diff * sum / 2;
}
