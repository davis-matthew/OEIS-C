/*
 * A382944  Table read by rows: T(n, k) = valuation(n, k) for k >= 2, 1 for k
 *          = 1 and 0^n for k = 0.
 * https://oeis.org/A382944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read the triangle T(row, col) row by row, where * T(row,
 *            col) = 0^row if col = 0, * 1 if col = 1, * exponent e such that
 *            col^e divides row otherwise
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static /*inline*/ idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Increase high until triangular number exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 base = (unsigned __int128)mid * (mid + 1) / 2;
        if (base <= n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;          /* row index */
}

seq_t a(idx_t n)
{
    /* locate row r and column k (k = 0 .. r) */
    idx_t r = find_row(n);
    unsigned __int128 base128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)base128;

    if (k == 0) {            /* 0^r */
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }
    if (k == 1) {
        return (seq_t)1;
    }

    /* k >= 2: compute exponent of k dividing r */
    idx_t temp = r;
    seq_t cnt = 0;
    while (temp != 0 && temp % k == 0) {
        temp /= k;
        cnt++;
    }
    return cnt;
}
