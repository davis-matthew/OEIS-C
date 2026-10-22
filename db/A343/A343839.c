/*
 * A343839  Semi-one numbers: Positive integers k such that exactly half of
 *          the integers 1..k have a 1 in their decimal expansion.
 * https://oeis.org/A343839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   brute force search for semi‑one numbers
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   has_digit_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_digit_one.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1 */

    static seq_t semi[16];
    static idx_t found = 0;
    static idx_t k = 1;
    static idx_t yes = 0;
    static bool initialized = false;
    static const idx_t LIMIT = 1062880;   /* largest known semi‑one number */

    if (!initialized) {
        initialized = true;
        k = 1;
        yes = 0;
        found = 0;
    }

    /* If we've already found enough terms, return the nth one */
    if (found >= n) return semi[n-1];

    /* Search for further semi‑one numbers */
    while (found < n && found < 16 && k <= LIMIT) {
        if (has_digit_one(k)) yes += 1;

        if (yes * 2 == k && found < 16) {
            semi[found++] = (seq_t)k;
        }
        k += 1;
    }

    /* If we found the requested term, return it; otherwise return 0 */
    if (found >= n) return semi[n-1];
    return 0;
}
