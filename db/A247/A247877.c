/*
 * A247877  Least prime factor of A247683.
 * https://oeis.org/A247877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least prime factor of A247683(n), where A247683 lists the
 *            odd composites == 7 (mod 9), i.e. composites of the form 18k+7.
 *            The code finds them by search and trial division
 * Range:     u8 n <= 6, u16 n <= 2630, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 9960 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = 7;
    while (1) {
        /* cand = 18k+7: odd, == 7 mod 9, so never divisible by 2 or 3 */
        seq_t d = 5;
        bool comp = false;
        while (d * d <= cand) {
            if (cand % d == 0) { comp = true; break; }
            d += 2;
        }
        if (comp) {
            ++found;
            if (found == n) return d;
        }
        cand += 18;
    }
}
