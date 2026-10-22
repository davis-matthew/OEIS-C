/*
 * A371031  Number of distinct integers resulting from adding an n-digit
 *          non-multiple of 10 and its reverse.
 * https://oeis.org/A371031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(1)=9; a(2)=17; for n≥3: * if n is even: a(n)=17·19^((n/2)-1) *
 *            if n is odd : a(n)=170·19^((n-3)/2) * Recurrence: for n≥4,
 *            a(n)=19·a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 9;
    if (n == 2) return 17;
    seq_t p = 1;
    if ((n & 1) == 0) {          /* even n */
        idx_t exp = n / 2 - 1;
        for (idx_t i = 0; i < exp; ++i)
            p = p * 19;
        return 17 * p;
    } else {                      /* odd n (≥3) */
        idx_t exp = (n - 3) / 2;
        for (idx_t i = 0; i < exp; ++i)
            p = p * 19;
        return 170 * p;
    }
}
