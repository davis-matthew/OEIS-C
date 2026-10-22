/*
 * A327919  The 17 pure prime dates of each leap year of the form
 *          concatenate(month,day) with month and day also prime numbers.
 * https://oeis.org/A327919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, changed
 * Formula:   a(n) returns the nth pure prime date of a leap year, cycling
 *            with a period of 17. A pure prime date is a concatenation of a
 *            prime month (2, 3, 5, 7, 11) and a prime day within that month,
 *            such that the concatenated number is also prime. The 17 dates in
 *            a leap year are 23,211,223,229,37,311,313,31...
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Simple 64‑bit primality test; works for the values needed (≤1129). */
static bool is_prime_uint64(uint64_t n)
{
    if (n <= 1U) return false;
    if (n <= 3U) return true;
    if ((n & 1U) == 0U) return false;
    for (uint64_t i = 3U; i * i <= n; i += 2U)
        if (n % i == 0U) return false;
    return true;
}

/* Concatenate month and day. month and day are small (< 100). */
static uint64_t concat(uint64_t month, uint64_t day)
{
    uint64_t pow10 = (day < 10U) ? 10U : 100U;   /* 10^digits(day) */
    return month * pow10 + day;
}

seq_t a(idx_t n)
{
    static seq_t dates[17];
    static bool built = false;

    if (!built) {
        /* Prime months and corresponding calendar lengths in a leap year. */
        static const uint64_t months[5] = {2U, 3U, 5U, 7U, 11U};
        static const uint64_t limits[5] = {29U, 31U, 31U, 31U, 30U};

        size_t count = 0;
        for (size_t m = 0; m < 5 && count < 17; ++m) {
            uint64_t month = months[m];
            uint64_t limit = limits[m];
            for (uint64_t day = 2U; day <= limit && count < 17; ++day) {
                if (!is_prime_uint64(day))
                    continue;
                uint64_t cand = concat(month, day);
                if (!is_prime_uint64(cand))
                    continue;
                dates[count++] = (seq_t)cand;
            }
        }
        built = true;
    }

    idx_t idx = (n - 1) % 17U;
    return dates[idx];
}
