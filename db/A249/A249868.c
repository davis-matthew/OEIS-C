/*
 * A249868  Number of days between prime dates in a leap year; starting with
 *          Feb 2 and ending with Feb 2 in the following year.
 * https://oeis.org/A249868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of days between successive prime dates in a full
 *            leap‑year cycle (period 53)
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for the offset */

    static seq_t gaps[53];
    static bool init = false;

    if (!init) {
        /* month lengths in a leap year */
        const idx_t month_days[12] = {
            31, 29, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };
        idx_t cum[13];
        cum[0] = 0;
        for (idx_t i = 0; i < 12; ++i)
            cum[i + 1] = cum[i] + month_days[i];

        /* base offset of the first prime date (Feb 2) */
        const idx_t base_offset = cum[1] + 1;   /* day number −1 */

        /* prime days and prime months */
        const idx_t prime_days[]  = {2,3,5,7,11,13,17,19,23,29,31};
        const idx_t prime_months[] = {2,3,5,7,11};

        idx_t offsets[54];
        idx_t count = 0;

        for (idx_t mi = 0; mi < sizeof(prime_months)/sizeof(prime_months[0]); ++mi) {
            idx_t m = prime_months[mi];            /* month (1‑based) */
            idx_t md = month_days[m - 1];          /* days in month */
            for (idx_t di = 0; di < sizeof(prime_days)/sizeof(prime_days[0]); ++di) {
                idx_t d = prime_days[di];
                if (d > md) break;
                offsets[count++] = cum[m - 1] + (d - 1);
            }
        }
        /* add Feb 2 of the following year */
        offsets[count++] = cum[12] + 31 + 1;       /* 366 + 31 + 1 = 398 */

        /* compute the gaps between consecutive dates */
        for (idx_t i = 1; i < count; ++i)
            gaps[i - 1] = (seq_t)(offsets[i] - offsets[i - 1]);

        init = true;
    }

    /* the sequence is periodic with period 53 */
    return gaps[(n - 1) % 53];
}
