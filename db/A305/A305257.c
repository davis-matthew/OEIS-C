/*
 * A305257  If pd(x) is the product of the digits of the number x and sd(x)
 *          the sum of the digits of the number x then the sequence lists all
 *          the positive numbers n for which pd(n) = sd(n) and sd(pd(n)) =
 *          pd(sd(n)).
 * https://oeis.org/A305257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th positive integer n such that pd(n) = sd(n) and
 *            sd(pd(n)) = pd(sd(n)) (OEIS A305257)
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    /* The sequence has exactly 48 terms. Build the list on first
       call and reuse it for later calls. */
    static seq_t seq[48];
    static size_t len = 0;

    if (len == 0)
    {
        uint64_t cand = 1;
        while (len < 48)
        {
            seq_t v = (seq_t)cand;
            seq_t s = digit_sum(v);
            if (s == 0)          /* sum 0 only possible with digit 0 */
            {
                cand++;
                continue;
            }
            seq_t p = digit_product(v);
            if (s == p && digit_sum(p) == digit_product(s))
                seq[len++] = v;
            cand++;
        }
    }

    if (n == 0 || n > len)
        return (seq_t)0;   /* undefined for out‑of‑range indices */

    return seq[n - 1];
}
