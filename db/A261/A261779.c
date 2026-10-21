/*
 * A261779  a(n) = ceiling((n-1)! / n).
 * https://oeis.org/A261779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n-1)! / n) Computation is done without forming
 *            (n-1)! by cancelling the divisor n progressively
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Handle n==1 directly (also works generically) */
    if (n == 0) {
        return 0;      /* offset guarantees n>=1, this is safe guard */
    }

    seq_t result = 1;
    idx_t   denom = n;          /* remaining part of n not cancelled */

    for (idx_t i = 1; i <= n - 1; ++i) {
        idx_t g = _gcd_idx(i, denom);
        denom /= g;
        result *= (seq_t)(i / g);
    }

    /* ceil(result / denom) = (result + denom - 1) / denom */
    return (result + (seq_t)denom - 1) / (seq_t)denom;
}
