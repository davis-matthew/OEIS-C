/*
 * A327346  The 74 prime dates of each year of the form concatenate(day,month)
 *          without leading zeros for month (and day), sorted by date,
 *          including duplicates.
 * https://oeis.org/A327346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, changed
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A327346:  The 74 prime dates of each year formed by
 * concatenating day and month (without leading zeros for either).
 * For the 5 months 1,3,7,9,11 the days run 1..maxdays (Jan,Mar,Jul,Sep,Nov).
 * The dates are sorted by month, then by day.
 * The sequence repeats every 74 terms (one block per year). */

#include <stdbool.h>
#include <stddef.h>

/* Test whether a small integer (≤ 3011) is prime. */
static bool is_prime(seq_t x)
{
    unsigned long long v = (unsigned long long)x;
    if (v < 2ull) return false;
    if (v % 2ull == 0ull) return v == 2ull;
    for (unsigned long long d = 3ull; d * d <= v; d += 2ull)
        if (v % d == 0ull) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static array of the 74 prime dates */
    static seq_t seq[74];
    static bool built = false;

    if (!built) {
        built = true;
        const int months[5]   = {1, 3, 7, 9, 11};
        const int maxdays[5]  = {31, 31, 31, 30, 30};
        int idx = 0; /* position in seq */

        for (int mi = 0; mi < 5; ++mi) {
            int month = months[mi];
            int mult  = (month < 10) ? 10 : 100;  /* 1‑digit or 2‑digit month */

            for (int day = 1; day <= maxdays[mi]; ++day) {
                seq_t val = (seq_t)day * (seq_t)mult + (seq_t)month;
                if (is_prime(val)) {
                    seq[idx++] = val;
                }
            }
        }
        /* idx should now be 74 */
    }

    /* return the (n‑1)th term modulo 74 */
    idx_t pos = (n - 1) % (idx_t)74;
    return seq[pos];
}
