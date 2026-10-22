/*
 * A358498  a(n) = Sum_{k=0..floor(n/3)} (n-3*k)!.
 * https://oeis.org/A358498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} (n-3k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Computes the sum of factorials of n, n-3, n-6, …
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t m = n;

    /* factorial of current m */
    seq_t fac = 1;
    for (idx_t i = 2; i <= m; ++i) {
        fac *= (seq_t)i;
    }

    seq_t sum = 0;
    while (1) {
        sum += fac;
        if (m < 3) break;

        /* divide fac by (m-2)*(m-1)*m to obtain (m-3)! */
        seq_t prod = ((seq_t)(m - 2)) * (seq_t)(m - 1) * (seq_t)m;
        fac /= prod;
        m -= 3;
    }
    return sum;
}
