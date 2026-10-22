/*
 * A364881  First significant digit of the decimal expansion of n/(2^n).
 * https://oeis.org/A364881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = floor( (n * 5^n) / 10^floor(log10(n * 5^n)) ), i.e. the
 *            first decimal digit of n*5^n (equivalently n/2^n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the first significant digit of n/2^n, which equals
   the first digit of n*5^n.  This implementation uses only
   integer arithmetic and works for all builds.  */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n=0 is undefined, return 0 */ 
        return (seq_t)0;

    /* Mantissa that keeps the most significant digits only.  */
    uint64_t mant = (uint64_t)n;          /* start with n */
    /* Limit so that mant never overflows 64‑bit integer when multiplied by 5. */
    const uint64_t LIMIT = 1000000000000000000ULL;  /* 10^18 */

    for (idx_t i = 0; i < n; ++i) {
        mant *= 5ULL;                /* multiply by 5^i */
        if (mant >= LIMIT)           /* scale down if too many digits */
            mant /= 10ULL;
    }

    /* Extract the leading decimal digit from mant. */
    while (mant >= 10ULL)
        mant /= 10ULL;

    return (seq_t)mant;
}
