/*
 * A358935  a(n) is the least k > 0 such that fusc(n) = fusc(n + k) or fusc(n)
 *          = fusc(n - k) (provided that n - k >= 0), where "fusc" is Stern's
 *          diatomic series (A002487).
 * https://oeis.org/A358935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that fusc(n) = fusc(n+k) or fusc(n) =
 *            fusc(n-k)
 * Range:     u8 n <= 511, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   fusc_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fusc_calc.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n < 1 but harmless      */
    seq_t fn = fusc_calc(n);
    for (idx_t k = 1; ; ++k)
    {
        if (fusc_calc(n + k) == fn) return k;   /* forward match   */
        if (n >= k && fusc_calc(n - k) == fn)  /* backward match  */
            return k;
    }
}
