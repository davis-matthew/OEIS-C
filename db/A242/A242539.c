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
 * Helpers:   first_day_wday, has_53_sundays, is_leap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/first_day_wday.h"
#include "../utils/has_53_sundays.h"
#include "../utils/is_leap.h"

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
