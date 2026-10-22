/*
 * A327349  The 67 prime dates in each leap year of the form concatenate
 *          (month,day) without leading zeros for days.
 * https://oeis.org/A327349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, changed
 * Formula:   a(n) = the n-th prime date in a leap year
 * Range:     u8 n <= 15, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function lists the 67 dates that are prime when written as
   month concatenated with day (no leading zeros on the day).
   The dates are computed once and stored in a static array. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* 67 prime dates in a leap year */
    static bool ready = false;
    static seq_t prime_dates[67];
    static size_t prime_count = 0;

    if (!ready) {
        /* Days in each month for a leap year */
        static const int days_in_month[13] = {
            0, 31, 29, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        size_t k = 0;
        for (int m = 1; m <= 12; ++m) {
            for (int d = 1; d <= days_in_month[m]; ++d) {
                /* form the concatenated number: month*base + day */
                /* base = 10 if d < 10 else 100 */
                seq_t v = (seq_t)m *
                           ((d < 10) ? (seq_t)10 : (seq_t)100) +
                           (seq_t)d;
                if (is_prime_big(v) && k < 67) {
                    prime_dates[k++] = v;
                }
            }
        }
        prime_count = k;   /* should be 67 */
        ready = true;
    }

    if (n == 0 || prime_count == 0)
        return 0;

    /* 1‑based index n; return 0 for n greater than the available primes */
    if (n > prime_count)
        return 0;

    return prime_dates[(size_t)(n - 1)];
}
