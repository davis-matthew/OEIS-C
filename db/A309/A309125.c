/*
 * A309125  a(n) = n + 2^2 * floor(n/2^2) + 3^2 * floor(n/3^2) + 4^2 *
 *          floor(n/4^2) + ...
 * https://oeis.org/A309125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊√n⌋} k^2 * ⌊n/k^2⌋
 * Range:     u8 n <= 48, u16 n <= 1799, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root of a 64‑bit unsigned value */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;          /* sqrt(UINT64_MAX) is < 2^32 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && mid > n / mid)     /* mid^2 > n  --> mid too big */
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;                           /* floor of √n */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* offset 1 – guard for n=0 */

    seq_t sum = 0;
    idx_t kmax = int_sqrt(n);
    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t ksq = k * k;                /* k^2 fits in idx_t (≤ n) */
        idx_t q   = n / ksq;
        sum += (seq_t)ksq * (seq_t)q;     /* accumulation in seq_t */
    }
    return sum;
}
