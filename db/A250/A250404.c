/*
 * A250404  Numbers k such that the set of all distinct values of phi of all
 *          divisors of k equals the set of all proper divisors of k+1 where
 *          phi is the Euler totient function (A000010).
 * https://oeis.org/A250404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = 1 (n=1), 2 (n=2), 3 (n=3); for n≥4 a(n) = (a(n-1)+1)^2 –
 *            1. (OEIS A250404)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;
    if (n == 3)
        return (seq_t)3;

    seq_t prev = (seq_t)3;          /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t = prev + (seq_t)1;  /* a(n-1)+1  */
        prev = t * t - (seq_t)1;   /* (a(n-1)+1)^2 – 1  */
    }
    return prev;
}
