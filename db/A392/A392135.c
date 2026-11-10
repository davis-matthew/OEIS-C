/*
 * A392135  Third column of A391957.
 * https://oeis.org/A392135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} v_5(k)*(4k-2n-1) = 4*Sum_{j>=1}
 *            5^j*t_j*(t_j+1)/2 - (2n+1)*Sum_{j>=1} t_j, t_j = floor(n/5^j)
 * Range:     u8 n <= 74, u16 n <= 7499, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9597 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t S = 0;   /* sum of k*v5(k) for k<=n */
    seq_t F = 0;   /* sum of v5(k) = v5(n!) */
    idx_t q = 5;
    while (q <= n) {
        seq_t t = (seq_t)(n / q);
        S += (seq_t)q * (t * (t + 1) / 2);
        F += t;
        if (q > n / 5) break;
        q *= 5;
    }
    return (seq_t)4 * S - ((seq_t)2 * (seq_t)n + 1) * F;
}
