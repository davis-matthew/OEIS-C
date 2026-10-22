/*
 * A323607  Triangular array: row n is the list of numbers from 1 to n, sorted
 *          in Sharkovsky order.
 * https://oeis.org/A323607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   sequence A323607 – concatenation of rows 1,2,3,… where each row
 *            n contains the numbers 1…n in Sharkovsky order
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the row number r such that T(r‑1) < n ≤ T(r), where
   T(k) = k*(k+1)/2.  Uses only integer arithmetic. */
static inline idx_t _row_of(idx_t n)
{
    idx_t low = 1, high = 1;
    while (((unsigned __int128)high * (high + 1) / 2) < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1) / 2) >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = _row_of(n);

    /* number of terms before this row */
    unsigned __int128 prev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t pos = n - (idx_t)prev128 - 1;   /* 0‑based index inside row r */
    idx_t idx = 0;                        /* position we are generating */

    /* odd numbers > 1 in ascending order */
    for (idx_t t = 3; t <= r; t += 2) {
        if (idx == pos) return (seq_t)t;
        ++idx;
    }

    /* numbers of the form 2^k * odd (k ≥ 1), in ascending odd order,
       then all k's in increasing order of k. */
    for (idx_t power = 2; power <= r; ) {
        for (idx_t t = 3; t <= r; t += 2) {
            if (t > r / power) break;
            idx_t val = power * t;
            if (idx == pos) return (seq_t)val;
            ++idx;
        }
        if (power > r / 2) break;
        power <<= 1;
    }

    /* powers of two in descending order */
    idx_t power = 1;
    while (power <= r / 2) power <<= 1;      /* largest power of two ≤ r */
    for (idx_t p = power; p >= 2; p >>= 1) {
        if (idx == pos) return (seq_t)p;
        ++idx;
    }

    /* final element 1 */
    return (seq_t)1;
}
