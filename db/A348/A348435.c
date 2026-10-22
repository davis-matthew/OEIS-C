/*
 * A348435  Decimal expansion of (2/3)*e in Coulombs, two thirds of the
 *          elementary charge.
 * https://oeis.org/A348435
 *
 * Offset:    -18    (a(-18) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   not stated
 * Range:     u8 n <= -9, u16 n <= -9, u32 n <= -9, u64 n <= -9, u128 n <= -9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348435: decimal digits of (2/3)*1.602176634 (coefficient 1.068117756). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Values are only defined for n ≥ -18; indices beyond the 10 available
       digits return 0, matching the infinite‐zero tail of the decimal
       expansion. */
    if (n < -18)
        return 0;
    if (n > -9)
        return 0;

    static seq_t digit[10];
    static int init = 0;
    if (!init) {
        /* Compute the integer representation of 1.068117756 * 10^9:
           (2/3) * 1.602176634 = 3204353268 / 3 = 1068117756.   */
        unsigned long long num   = 1602176634ULL * 2ULL;  /* 2*1.602176634*10^9 */
        unsigned long long q     = num / 3ULL;            /* floor division   */
        unsigned long long tmp   = q;
        /* Extract the 10 decimal digits, most significant first. */
        for (int i = 9; i >= 0; --i) {
            digit[i] = (seq_t)(tmp % 10ULL);
            tmp /= 10ULL;
        }
        init = 1;
    }

    int idx = (int)(n + 18);  /* 0 … 9 */
    return digit[idx];
}
