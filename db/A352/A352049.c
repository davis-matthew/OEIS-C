/*
 * A352049  Sum of the cubes of the divisor complements of the odd proper
 *          divisors of n.
 * https://oeis.org/A352049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * Σ_{d|n, d<n, d odd} 1/d^3 equivalently a(n) =
 *            Σ_{odd d|n, d<n} (n/d)^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1604, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)                    /* not needed for OEIS offset but safe */
        return 0;
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {   /* iterate up to √n safely */
        if (n % d == 0) {
            if (d < n && (d & 1)) {        /* odd proper divisor d */
                seq_t m = ((seq_t)n) / d;  /* m = n/d */
                sum += m * m * m;
            }
            idx_t e = n / d;                /* paired divisor */
            if (e != d && e < n && (e & 1)) {
                seq_t d_seq = (seq_t)d;      /* m = n/e = d */
                sum += d_seq * d_seq * d_seq;
            }
        }
    }
    return sum;
}
