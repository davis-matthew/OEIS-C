/*
 * A377293  Number of days where month plus day equals n, in a non-leap year
 *          in the Gregorian calendar.
 * https://oeis.org/A377293
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, dumb, easy, fini, full
 * Formula:   a(n) = number of days in a non‑leap Gregorian year whose month
 *            number plus day number equals n
 * Range:     u8 n <= 43, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t days[12] =
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    seq_t cnt = 0;
    for (idx_t m = 1; m <= 12; ++m) {
        if (n <= m)          /* day would be zero or negative */
            continue;
        idx_t d = n - m;     /* candidate day */
        if (d <= days[m - 1])
            ++cnt;
    }
    return cnt;
}
