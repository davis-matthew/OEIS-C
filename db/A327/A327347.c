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
 * Validated: full, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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
