/*
 * A261465  a(n) = prime(n+1)^2 - prime(n).
 * https://oeis.org/A261465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n+1)^2 - prime(n)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; for n >= 1 compute prime(n+1)^2 - prime(n) */
    if (n < 1)
        return 0;               /* undefined for n < 1; return 0 */

    seq_t p_prev = (seq_t)get_prime(n);     /* prime(n)   */
    seq_t p_next = (seq_t)get_prime(n + 1); /* prime(n+1) */

    return p_next * p_next - p_prev;       /* prime(n+1)^2 - prime(n) */
}
