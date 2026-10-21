/*
 * A268287  a(n) shows how many times the sum of the hours and minutes is
 *          prime in an n-hour period starting from midnight.
 * https://oeis.org/A268287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A268287.
   The counts per hour are periodic with period 24.
   For each hour h (0‑23) let c[h] be the number of minutes m∈[0,59]
   such that h+m is prime.  Then
       a(n) = sum_{k=0}^{n-1} c[k mod 24].
   For n ≥ 0 the function returns this cumulative sum. */

#include <stdbool.h>

/* prime test for small integers (≤ 82) */
static bool isPrimeSmall(int n)
{
    if (n < 2) return false;
    for (int d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static table of counts per hour, initialized once */
    static seq_t hour_counts[24];
    static seq_t sum_24 = 0;
    static bool initialized = false;

    if (!initialized) {
        for (int h = 0; h < 24; ++h) {
            seq_t cnt = 0;
            for (int m = 0; m < 60; ++m)
                if (isPrimeSmall(h + m))
                    ++cnt;
            hour_counts[h] = cnt;
            sum_24 += cnt;
        }
        initialized = true;
    }

    seq_t result = (seq_t)(n / 24) * sum_24;
    idx_t rem = n % 24;
    for (idx_t i = 0; i < rem; ++i)
        result += hour_counts[i];

    return result;
}
