/*
 * A340832  Number of factorizations of n into factors > 1 with odd least
 *          factor.
 * https://oeis.org/A340832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of factorizations of n into factors > 1 with odd least
 *            factor
 * Range:     u8 n <= 9211, u16 n <= 9211, u32 n <= 9211, u64 n <= 9211,
 *            u128 n <= 9211, big any n
 * Helpers:   count_with_min
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_with_min.h"

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
