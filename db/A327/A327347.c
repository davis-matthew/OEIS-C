/*
 * A327347  The 54 prime dates of each year of the form concatenate(day,month)
 *          with leading zero for months 1, 3, 7, 9 (no leading zero for
 *          days).
 * https://oeis.org/A327347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, changed
 * Formula:   a(n) = p[(n‑1) mod 54] where p is the list of prime dates. Each
 *            date is obtained by concatenating the day (no leading zero) with
 *            the month (01, 03, 07, 09, 11). The list is sorted by month and
 *            day
 * Range:     u8 n <= 1, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

static seq_t prime_dates[60];
static size_t prime_cnt = 0;

static void init_prime_dates(void)
{
    if (prime_cnt != 0) return;                      /* already initialized */

    const idx_t months[5]     = {1, 3, 7, 9, 11};
    const idx_t days_in_month[5] = {31, 31, 31, 30, 30};

    for (size_t mi = 0; mi < 5; ++mi) {
        idx_t month = months[mi];
        idx_t days  = days_in_month[mi];
        for (idx_t day = 1; day <= days; ++day) {
            seq_t val = day * ((seq_t)100) + month;   /* concatenate day and month */
            if (isPrimeGeneric(val)) {
                prime_dates[prime_cnt++] = val;
            }
        }
    }
    /* prime_cnt should now be 54 */
}

seq_t a(idx_t n)
{
    init_prime_dates();
    idx_t idx = (n - 1) % (idx_t)prime_cnt;  /* periodicity of 54 */
    return prime_dates[idx];
}
