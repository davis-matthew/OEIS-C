/*
 * A273909  Let p = prime(n) and q = prime(n+1), then a(n) = p*q - p^2 - 2*q.
 * https://oeis.org/A273909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n) * prime(n+1) - prime(n)^2 - 2 * prime(n+1) =
 *            (g-2)*p - 2*g, * where p = prime(n), q = prime(n+1), g = q - p
 * Range:     i8 n <= 14, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this code uses the utility function get_prime(k) to retrieve the k‑th prime. */

#include "../utils/get_prime.h"

extern seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 (behaviour below offset not important). */
    if (n < 1u) return 0;

    /* Retrieve the required primes.  get_prime returns uint32_t, which is safe for n up to 10000. */
    uint32_t p_u = get_prime(n);
    uint32_t q_u = get_prime(n + 1);

    /* Operate in seq_t to avoid overflow on types such as int64_t or __int128. */
    seq_t p = (seq_t)p_u;
    seq_t q = (seq_t)q_u;
    seq_t g = q - p;                  /* the prime gap */
    seq_t result = (g - 2) * p - 2 * g;
    return result;
}
