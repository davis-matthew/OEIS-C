/*
 * A363249  Leading digit of 9^n.
 * https://oeis.org/A363249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = leading digit of 9^n (i.e. a(n) = floor(10^{fractional
 *            part of n·log10(9)}) ) Computation is performed with integer
 *            arithmetic only; no floating point or <math.h> functions are
 *            used
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 44, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Choose the number of mantissa digits to keep.
       The choice guarantees that 9 * (10^m – 1) fits within seq_t
       and that 10^m itself also fits.  The values are
       valid for all common sizes of seq_t. */
    int m;
    size_t sz = sizeof(seq_t);
    if (sz <= 1)        m = 1;   /* 8‑bit integer: 0‑255  */
    else if (sz <= 2)  m = 3;   /* 16‑bit integer: 0‑65535 */
    else if (sz <= 4)  m = 8;   /* 32‑bit integer: 0‑4294967295 */
    else if (sz <= 8)  m = 18;  /* 64‑bit integer: 0‑18446744073709551615 */
    else               m = 38;  /* 128‑bit integer: 0‑340282366920938463463374607431768211455 */

    /* Compute pow10 = 10^m as a seq_t value. */
    seq_t pow10 = 1;
    for (int i = 0; i < m; ++i) {
        pow10 *= (seq_t)10;
    }

    /* Scientific mantissa: keep the most significant m digits.
       After each multiplication by 9, we discard the trailing digit
       to keep the mantissa within the range [1, pow10-1]. */
    seq_t mant = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        mant *= (seq_t)9;
        if (mant >= pow10) {
            mant /= (seq_t)10;   /* remove a trailing digit */
        }
    }

    /* Extract the leading digit of the mantissa. */
    seq_t leading = mant;
    while (leading >= (seq_t)10) {
        leading /= (seq_t)10;
    }
    return leading;
}
