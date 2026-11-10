/*
 * A276617  Square array A(n,k) = A276955(n,k)/k!, read by descending
 *          antidiagonals as A(1,1), A(1,2), A(2,1), A(1,3), A(2,2), A(3,1),
 *          etc.
 * https://oeis.org/A276617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   A(n,k) = A276955(n,k)/k!; A276955 row n starts at the n-th
 *            number not yet used by earlier rows (mex) and each next column
 *            is the factorial-base left shift (A153880); a(n) read by
 *            descending antidiagonals
 * Range:     u8 n <= 161, u16 n <= 1614, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Validated: standard, pass, 107 terms through n = 7028 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A276617_shift_one(seq_t x)
{
    unsigned digits[160];
    int len = 0;
    seq_t t = x;
    unsigned f = 2;
    while (t != (seq_t)0)
    {
        digits[len++] = (unsigned)(t % (seq_t)f);
        t /= (seq_t)f;
        ++f;
    }
    seq_t result = 0;
    seq_t fact = 1;
    for (int i = 0; i < len; ++i)
    {
        fact *= (seq_t)(i + 2);
        result += (seq_t)digits[i] * fact;
    }
    return result;
}

#define A276617_MAX_ROWS 10000
static seq_t A276617_first_col[A276617_MAX_ROWS + 1];
static idx_t A276617_cnt = 0;
static seq_t A276617_next = 1;

static bool A276617_is_used(seq_t cand, idx_t upto_row)
{
    for (idx_t r = 1; r < upto_row; ++r)
    {
        seq_t cur = A276617_first_col[r];
        if (cur == cand)
            return true;
        while (true)
        {
            seq_t nxt = A276617_shift_one(cur);
            if (nxt == cand)
                return true;
            if (nxt > cand || nxt <= cur)
                break;
            cur = nxt;
        }
    }
    return false;
}

static void A276617_generate(idx_t target)
{
    while (A276617_cnt < target)
    {
        while (A276617_is_used(A276617_next, A276617_cnt + 1))
            ++A276617_next;
        ++A276617_cnt;
        A276617_first_col[A276617_cnt] = A276617_next;
        ++A276617_next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t s = 1;
    while (s * (s + 1) / 2 < n)
        ++s;
    idx_t row = n - (s - 1) * s / 2;
    idx_t col = s - row + 1;
    idx_t sh = col - 1;

    A276617_generate(row);

    /* digits of base in factorial base; digit i has weight
       (i+sh+1)!/(sh+1)! after dividing by col! */
    seq_t t = A276617_first_col[row];
    seq_t result = 0;
    seq_t w = 1;
    idx_t f = 2;
    idx_t i = 0;
    while (t != (seq_t)0)
    {
        seq_t d = t % (seq_t)f;
        t /= (seq_t)f;
        ++f;
        if (i > 0)
            w *= (seq_t)(sh + i + 1);
        result += d * w;
        ++i;
    }
    return result;
}
