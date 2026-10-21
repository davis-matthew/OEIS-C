/*
 * A249867  Number of days between prime dates in a non-leap year; starting
 *          with Feb 2 and ending with Feb 2 in the following year.
 * https://oeis.org/A249867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of days between successive prime dates in a
 *            non‑leap year, starting with Feb 2 and ending with Feb 2 of the
 *            following year (period 52)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define NUM_GAPS 52

/* Number of days in each month (non‑leap year).  monthlen[0] is unused. */
static const int monthlen[13] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/* Cumulative days before the first day of each month.  before_month[0] unused. */
static const int before_month[13] = {
    0,   0,   31,  59,  90, 120, 151, 181, 212, 243,
    273, 304, 334
};

/* Prime month numbers (except February is counted for its prime days). */
static const int prime_months[5] = { 2, 3, 5, 7, 11 };

/* Prime day numbers up to 31. */
static const int prime_days[11] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31 };

static bool initialized = false;
static seq_t gaps[NUM_GAPS];

/* Compute all gaps once and store them in the static array. */
static void compute_gaps(void)
{
    int gap_index = 0;
    int prev_day = before_month[2] + 2;   /* Day number of Feb 2. */

    /* Iterate through each prime month and its prime days. */
    for (int mi = 0; mi < 5; ++mi) {
        int month = prime_months[mi];
        for (int di = 0; di < 11; ++di) {
            int day = prime_days[di];
            if (day > monthlen[month])
                continue;                       /* Day does not exist in this month. */
            if (month == 2 && day == 2)
                continue;                       /* Skip the starting date. */

            int cur_day = before_month[month] + day;
            gaps[gap_index++] = (seq_t)(cur_day - prev_day);
            prev_day = cur_day;
        }
    }

    /* Gap from the last prime date of the year (Nov 29) to
       Feb 2 of the following year. */
    int last_gap = 365 - prev_day + (before_month[2] + 2);
    gaps[gap_index++] = (seq_t)last_gap;

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        compute_gaps();

    /* The sequence repeats every NUM_GAPS entries. */
    idx_t idx = (n - 1) % (idx_t)NUM_GAPS;
    return gaps[(size_t)idx];
}
