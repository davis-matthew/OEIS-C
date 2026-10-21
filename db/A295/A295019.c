/*
 * A295019  Squares whose largest digit is 9.
 * https://oeis.org/A295019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A295009(n)^2 (squares whose largest digit is 9)
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_digit9(seq_t x)
{
    const seq_t nine = (seq_t)9;
    const seq_t ten = (seq_t)10;
    while (x)
    {
        if ((x % ten) == nine)
            return true;
        x /= ten;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    idx_t count = 0;
    seq_t m = (seq_t)1;
    while (true)
    {
        seq_t sq = m * m;            /* square of current integer */
        if (has_digit9(sq))
        {
            ++count;
            if (count == n)
                return sq;
        }
        ++m;
    }
}
