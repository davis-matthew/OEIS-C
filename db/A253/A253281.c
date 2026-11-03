/*
 * A253281  Triangular array read by rows: T(h,k) = number of steps from (h,k)
 *          to (0,0), where allowable steps are as follows: (x,y) -> (x-r, y)
 *          if r > 0, and (x,y) -> (y, r/3) otherwise, where r = x mod 3.
 * https://oeis.org/A253281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = number of steps to reach (0,0) from the coordinate (h,k)
 *            that is the n‑th term of the triangular array read by rows.
 *            Steps: if x mod 3 > 0 then (x,y) -> (x-(x mod 3), y); else (x,y)
 *            -> (y, x/3)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t steps_to_zero(seq_t x, seq_t y)
{
    seq_t steps = 0;
    while (x != 0 || y != 0)
    {
        seq_t r = x % (seq_t)3;
        if (r > 0)
            x -= r;                     /* subtract remainder */
        else
        {
            seq_t tmp = x / (seq_t)3;   /* x is divisible by 3 here */
            x = y;
            y = tmp;                    /* swap and divide */
        }
        ++steps;
    }
    return steps;
}

seq_t a(idx_t n)
{
    /* Find row h such that (h+1)(h+2)/2 >= n. */
    idx_t h = 0;
    while (((h + 1) * (h + 2) / 2) < n)
        ++h;

    /* Position j inside the row (0‑based). */
    idx_t prev_tri = h * (h + 1) / 2;   /* terms before row h */
    idx_t j = n - prev_tri - 1;        /* index inside row h */

    /* Convert to coordinates (x,y) = (h-j, j). */
    seq_t x = (seq_t)(h - j);
    seq_t y = (seq_t)j;

    /* Return the step count. */
    return steps_to_zero(x, y);
}
