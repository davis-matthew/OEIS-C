/*
 * A307560  a(n) = smallest m such that A307629(m) = n.
 * https://oeis.org/A307560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A307560
   a(n) = smallest m such that A307629(m) = n,
   where A307629(m) is the sum of all digit sums of unordered pairs
   of digits of m; equivalently (digits_count(m)-1)*digit_sum(m) = n.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find the smallest divisor d of n satisfying 9*d*(d+1) >= n. */
    idx_t d;
    for (d = 1; d <= n; ++d) {
        if (n % d == 0) {
            idx_t cond = 9 * d * (d + 1);
            if (cond >= n) break;
        }
    }

    /* Number of digits is L = d + 1, target digit sum is S = n/d. */
    idx_t L = d + 1;
    idx_t S = n / d;
    idx_t sum_left = S;

    seq_t result = 0;
    for (idx_t i = 0; i < L; ++i) {
        idx_t remaining = L - i - 1;
        idx_t max_for_rest = 9 * remaining;
        idx_t min_digit = (i == 0) ? 1 : 0;
        idx_t digit;

        if (sum_left > max_for_rest)
            digit = sum_left - max_for_rest;   /* must allocate some to this position */
        else
            digit = min_digit;                 /* keep it as small as possible */

        if (digit > 9) digit = 9;               /* safety guard */

        result = result * 10 + (seq_t)digit;
        sum_left -= digit;
    }

    return result;
}
