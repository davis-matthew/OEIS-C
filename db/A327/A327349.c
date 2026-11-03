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
 * Validated: full, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function lists the 67 dates that are prime when written as
   month concatenated with day (no leading zeros on the day).
   The dates are computed once and stored in a static array. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
