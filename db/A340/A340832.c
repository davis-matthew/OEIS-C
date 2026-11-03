/*
 * A340832  Number of factorizations of n into factors > 1 with odd least
 *          factor.
 * https://oeis.org/A340832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of factorizations of n into factors > 1 with odd least
 *            factor
 * Range:     u8 n <= 15119, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 124 terms through n = 9211 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper: count factorizations of 'n' where all factors are >= min_factor.
   The factors are considered in non‑decreasing order to avoid duplicates. */
static seq_t count_with_min(idx_t n, idx_t min_factor)
{
    seq_t total = 0;
    for (idx_t i = min_factor; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t rest = n / i;          /* rest >= i by loop condition */
            /* two‑factor factorization [i, rest] */
            ++total;
            /* deeper factorizations of 'rest' with new minimal factor 'i' */
            total += count_with_min(rest, i);
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n <= 1)                     /* 1 has no factorization into >1 factors */
        return 0;

    seq_t result = 0;

    /* trivial factorization [n] counts only if n is odd */
    if (n % 2 == 1)
        ++result;

    /* first factor must be odd and >1 */
    for (idx_t i = 3; i <= n / i; i += 2) {   /* odd i only, starting from 3 */
        if (n % i == 0) {
            idx_t rest = n / i;               /* rest >= i (since i <= √n) */
            /* factorization [i, rest] */
            ++result;
            /* deeper factorizations of 'rest' starting with factor i */
            result += count_with_min(rest, i);
        }
    }

    return result;
}
