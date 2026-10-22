/*
 * A375027  Number of occurrences of Easter Sunday on March 22, March 23, ...,
 *          April 25 during a 532-year Julian Easter cycle.
 * https://oeis.org/A375027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of occurrences of Easter Sunday on the date March
 *            22 + (n-1) days (or April dates) in any 532‑year Julian cycle.
 *            The cycle contains 35 distinct dates, each appearing 4, 8, 12,
 *            16, or 20 times
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t julian_easter_counts[35] = {0};
static bool julian_counts_computed = false;

/* Compute the 532‑year cycle once. */
static void compute_julian_counts(void)
{
    for (idx_t year = 1; year <= 532; ++year) {
        /* Julian Easter algorithm (see A348924). */
        idx_t a = year % 19;
        idx_t b = year % 4;
        idx_t c = year % 7;
        idx_t d = (19 * a + 15) % 30;
        idx_t e = (2 * b + 4 * c + 6 * d + 12) % 7;
        idx_t date = 22 + d + e;          /* day number starting at 1 */

        idx_t month, day;
        if (date > 31) {
            month = 4;
            day   = date - 31;
        } else {
            month = 3;
            day   = date;
        }

        /* Map month/day to the 1‑based index 1..35. */
        idx_t idx;
        if (month == 3)
            idx = day - 22 + 1;          /* 1‑based index for March dates */
        else
            idx = 10 + day;              /* 10 days in March 22‑31 + April day */

        /* Increment count for this date. */
        julian_easter_counts[idx - 1]++; /* counts are small (≤20) */
    }
    julian_counts_computed = true;
}

seq_t a(idx_t n)
{
    if (!julian_counts_computed)
        compute_julian_counts();

    /* The sequence is defined for n = 1..35.  Return 0 for larger n. */
    if (n < 1 || n > 35)
        return (seq_t)0;
    return julian_easter_counts[n - 1];
}
