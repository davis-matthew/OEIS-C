/*
 * A259143  Number of distinct digits needed to write the calendar date of
 *          n-th day of a non-leap year in decimal representation, without
 *          leading zeros.
 * https://oeis.org/A259143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = number of distinct digits in the decimal representation
 *            of the month and day of the n‑th day of a non‑leap year (year
 *            repeats every 365 days)
 * Range:     u8 n <= 365, u16 n <= 365, u32 n <= 365, u64 n <= 365,
 *            u128 n <= 365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const int month_len[12] = {
    31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
};

seq_t a(idx_t n)
{
    /* Map n to a day in the 365‑day year */
    idx_t d = ((n - 1) % 365) + 1;      /* day of year 1..365 */

    /* Find month and day of month */
    idx_t month = 1;                    /* months are 1‑based */
    while (d > (idx_t)month_len[month - 1]) {
        d -= (idx_t)month_len[month - 1];
        ++month;
    }
    idx_t day_of_month = d;             /* 1..31 */

    /* Count distinct decimal digits in month and day */
    unsigned char seen[10] = {0};
    idx_t temp;

    temp = month;
    while (temp > 0) {
        seen[temp % 10] = 1;
        temp /= 10;
    }

    temp = day_of_month;
    while (temp > 0) {
        seen[temp % 10] = 1;
        temp /= 10;
    }

    unsigned int count = 0;
    for (int i = 0; i < 10; ++i)
        if (seen[i])
            ++count;

    return (seq_t)count;               /* value is 1..4 */
}
