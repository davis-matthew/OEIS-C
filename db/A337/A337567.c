/*
 * A337567  Let a(0) = 1, k(0) = 1. For n >= 1; if a(n-1) + k(n-1) is a prime,
 *          then a(n) = a(n-1) + k(n-1), k(n) = k(n-1); else a(n) = a(n-1) +
 *          k(n-1) + 1, k(n) = k(n-1) + 1.
 * https://oeis.org/A337567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + k(n-1) if a(n-1)+k(n-1) is prime * a(n) = a(n-1)
 *            + k(n-1) + 1 otherwise, * where k(0)=1, a(0)=1
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Compute the n‑th term of A337567.  This implementation
 * iterates from 1 to n, testing primality with the
 * provided is_prime_big routine, which works for all
 * supported seq_t sizes.  No memoisation is required
 * for n≤10000 and the routine remains fast enough for
 * the required limits. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;   /* a(0) */
    seq_t k_val = (seq_t)1;   /* k(0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = a_val + k_val;
        if (is_prime_big(sum)) {
            a_val = sum;          /* prime, keep k_val unchanged */
        } else {
            a_val = sum + (seq_t)1;
            k_val = k_val + (seq_t)1;
        }
    }
    return a_val;
}
