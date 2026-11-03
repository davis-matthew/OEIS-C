/*
 * A360536  Analog of Rudin-Shapiro sum sequence A020990, based on counting
 *          patterns 00 instead of 11.
 * https://oeis.org/A360536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i=0..n} (-1)^(i + e(i)), where e(i) counts * the
 *            (possibly overlapping) occurrences of the pattern "00" * in the
 *            binary representation of i. e(0) = 0
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int count00(idx_t x)
{
    int e = 0;
    int prev = -1;          /* previous bit, -1 means none */
    while (x) {
        int bit = (int)(x & 1ULL);
        if (bit == 0 && prev == 0) e++;
        prev = bit;
        x >>= 1;
    }
    return e;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        int e = count00(i);
        idx_t parity = (i + (idx_t)e) & 1ULL;   /* 0: even, 1: odd */
        sum += (parity == 0) ? (seq_t)1 : (seq_t)(-1);
    }
    return sum;
}
