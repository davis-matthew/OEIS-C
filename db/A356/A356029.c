/*
 * A356029  a(n) = n! * Sum_{k=0..floor(n/2)} (n - 2*k)^k/(2^k * (n - 2*k)!).
 * https://oeis.org/A356029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n-2k)^k/(2^k * (n-2k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term   = 1;
        idx_t twos   = k;                     /* number of 2's we have to remove */

        /* multiply by (n-2k+1)*(n-2k+2)*...*n, removing factors of 2 as we go */
        for (idx_t j = n - 2 * k + 1; j <= n; ++j) {
            term *= (seq_t)j;
            while ((term & 1) == 0 && twos > 0) {
                term >>= 1;
                --twos;
            }
        }

        /* multiply by (n-2k)^k, again removing factors of 2 */
        seq_t w = (seq_t)(n - 2 * k);
        for (idx_t i = 0; i < k; ++i) {
            term *= w;
            while ((term & 1) == 0 && twos > 0) {
                term >>= 1;
                --twos;
            }
        }

        /* any remaining 2's (there should be none) */
        if (twos > 0) term >>= twos;

        result += term;
    }

    return result;
}
