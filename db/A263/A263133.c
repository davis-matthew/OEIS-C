/*
 * A263133  Numbers m such that binomial(4*m + 3, m) is odd.
 * https://oeis.org/A263133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A263132(n) – 1. a(n) is the nth integer m (with a(1)=0)
 *            such that binomial(4*m + 3, m) is odd. Equivalently, m &
 *            ~(4*m+3) == 0. The function simply scans m = 0,1,2,… until the
 *            nth such m is found
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for out‑of‑range calls */

    idx_t count = 0;
    seq_t m = (seq_t)0;

    for (;; ++m) {
        seq_t t = m * (seq_t)4 + (seq_t)3;
        if ( (m & ~t) == 0 ) {
            ++count;
            if (count == n) return m;
        }
    }
}
