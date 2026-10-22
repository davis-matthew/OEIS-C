/*
 * A374437  a(n) = [1 - (n + s)/2]_n * [1 - (n - s)/2]_n, where s = sqrt(-n*(2
 *          + n)) and [x]_n is the Pochhammer function.
 * https://oeis.org/A374437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n-1} ( ((2(k+1)-n)^2 + n(n+2)) / 4 ) * derived
 *            from a(n) = [1 - (n + √(-n(n+2)))/2]_n * * [1 - (n -
 *            √(-n(n+2)))/2]_n * (The division by 4 is performed at each step,
 *            keeping the * intermediate values small while keeping the result
 *            exact.)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    seq_t n_nplus2 = (seq_t)n * ((seq_t)n + 2);   /* n*(n+2) */

    for (idx_t k = 0; k < n; ++k) {
        /* (2(k+1)-n) may be negative; use signed 64‑bit. */
        int64_t temp = 2 * ((int64_t)k + 1) - (int64_t)n;
        seq_t val = (seq_t)(temp * temp) + n_nplus2;  /* numerator */
        /* val is always divisible by 4 */
        result *= val / 4;
    }

    return result;
}
