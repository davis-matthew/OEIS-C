/*
 * A327846  Full days remaining in the month on the Gregorian calendar
 *          starting at n-th day of a non-leap year.
 * https://oeis.org/A327846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = last_day_of_month(n) - n (for non‑leap year, days counted
 *            cyclically)
 * Range:     u8 n <= 365, u16 n <= 365, u32 n <= 365, u64 n <= 365,
 *            u128 n <= 365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* month starts (1-indexed) and month lengths for a non‑leap year */
    static const idx_t month_start[12] = {
        1,   32,   60,   91,
        121, 152,  182,  213,
        244, 274, 305,  335
    };
    static const idx_t month_len[12]  = {
        31, 28, 31, 30,
        31, 30, 31, 31,
        30, 31, 30, 31
    };

    /* make n cycle each 365‑day year */
    idx_t day = (n - 1) % 365 + 1;

    /* find the month for 'day' */
    for (int i = 0; i < 12; ++i) {
        idx_t month_end = month_start[i] + month_len[i] - 1;
        if (day <= month_end) {
            return (seq_t)(month_end - day);
        }
    }

    /* should never reach here */
    return (seq_t)0;
}
