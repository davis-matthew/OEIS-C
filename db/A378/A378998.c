/*
 * A378998  Number of trailing 1-bits in the binary representation of
 *          sigma(n).
 * https://oeis.org/A378998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v2(1 + sigma(n)) (number of trailing 1‑bits in sigma(n))
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9877 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    /* Compute sigma(n) */
    seq_t s = sigma_of(n);

    /* Count trailing 1‑bits: equivalent to v2(1 + sigma(n)) */
    seq_t t = s + 1;   /* 1 + sigma(n)  */
    seq_t cnt = 0;     /* result */

    /* While the lowest bit of t is 0, t is divisible by 2 */
    while ((t & 1) == 0) {
        t >>= 1;
        cnt++;
    }
    return cnt;
}
