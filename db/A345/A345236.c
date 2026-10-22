/*
 * A345236  Triangle read by rows: The rightmost column contains the terms of
 *          A002262 starting at A002262(1). Each time a column's value is zero
 *          (except for a(0)), the column to its left starts at the next term
 *          in A002262, or if that column does not yet exist, it starts at
 *          A002262(2).
 * https://oeis.org/A345236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A345236 is generated from a triangle where c(m) =
 *            floor((sqrt(9+8*m)-3)/2) and r(m) = (2*m - c(m)^2 - 3*c(m)) / 2.
 *            Each row m consists of r(m), r(c(m)), r(c(c(m))), … until c(*) =
 *            0. The global sequence a(n) is the triangle read by rows
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"

static idx_t c_of(idx_t m)
{
    /* 9+8*m fits in idx_t for any valid m. */
    idx_t d = 9 + 8 * m;
    idx_t s = int_sqrt(d);
    return (s - 3) / 2;
}

static seq_t r_of(idx_t m)
{
    idx_t c = c_of(m);
    /* Compute numerator in idx_t to avoid overflow in small seq_t types. */
    idx_t num = (idx_t)2 * m - (idx_t)c * c - (idx_t)3 * c;
    /* num is always even; division truncates to integer. */
    return (seq_t)(num / 2);
}

static idx_t row_len(idx_t m)
{
    idx_t len = 0;
    idx_t cur = m;
    do {
        ++len;
        cur = c_of(cur);
    } while (cur != 0);
    return len;
}

static seq_t row_val(idx_t m, idx_t pos)
{
    /* Build the chain m, c(m), c(c(m)), … until 0. */
    idx_t stack[64];
    idx_t n = 0;
    idx_t cur = m;
    do {
        stack[n++] = cur;
        cur = c_of(cur);
    } while (cur != 0);

    /* The rightmost element is r(m) (pos = n-1), so the leftmost is
       the first element of the chain. */
    idx_t idx = stack[n - 1 - pos];
    return r_of(idx);
}

seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    for (idx_t m = 0; ; ++m) {
        idx_t len = row_len(m);
        if (n < cumulative + len) {
            idx_t pos = n - cumulative;
            return row_val(m, pos);
        }
        cumulative += len;
    }
}
