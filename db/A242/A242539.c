/*
 * A242539  Years with 53 sundays in the Gregorian calendar.
 * https://oeis.org/A242539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th Gregorian year in which the calendar contains
 *            53 Sundays, i.e., the year starts on Sunday, or it is a leap
 *            year that starts on Saturday
 * Range:     u8 none, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline idx_t first_day_wday(idx_t y) {
    /* Zeller’s congruence for the Gregorian calendar.
       Return 0 for Sunday, 1 for Monday, …, 6 for Saturday. */
    idx_t ym1 = y - 1ULL;          /* treat Jan as month 13 of previous year */
    idx_t m = 13ULL;               /* month = 13 (January) */
    idx_t k = ym1 % 100ULL;        /* year of century */
    idx_t j = ym1 / 100ULL;        /* zero‑based century */
    /* h = 0 … Saturday, 1 … Sunday, … 6 … Friday */
    idx_t h = (1ULL + (13ULL * (m + 1ULL)) / 5ULL
               + k + k / 4ULL + j / 4ULL + 5ULL * j) % 7ULL;
    /* convert to 0=Sunday … 6=Saturday */
    return (h + 6ULL) % 7ULL;
}

static inline bool is_leap(idx_t y) {
    /* Gregorian leap rule: divisible by 4, not by 100 unless also divisible by 400 */
    return (y % 4ULL == 0ULL) && ((y % 100ULL != 0ULL) || (y % 400ULL == 0ULL));
}

static inline bool has_53_sundays(idx_t year) {
    idx_t w = first_day_wday(year);
    return (w == 0ULL) || (is_leap(year) && w == 6ULL);
}

seq_t a(idx_t n) {
    /* The first term a(1) is 1584.  We step through the calendar
       starting with year 1584 and collect the qualifying years
       until the requested index n is reached. */
    idx_t year = 1583ULL;          /* start just before the first term */
    while (1) {
        ++year;                    /* advance to the next year */
        if (has_53_sundays(year)) {
            if (--n == 0ULL) {
                return (seq_t)year;
            }
        }
    }
}
