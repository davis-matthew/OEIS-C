/*
 * A363265  Number of integer factorizations of n with a unique mode.
 * https://oeis.org/A363265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer factorizations of n with a unique mode
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t count_factorizations(idx_t rem, idx_t min_factor, idx_t *fac, int depth)
{
    seq_t cnt = 0;

    /* Try all possible divisors d, maintaining non‑decreasing order to avoid duplicates */
    for (idx_t d = min_factor; d * d <= rem; ++d) {
        if (rem % d == 0) {
            fac[depth] = d;
            cnt += count_factorizations(rem / d, d, fac, depth + 1);
        }
    }

    /* Final factor = rem itself, completing a factorization */
    if (rem >= min_factor) {
        fac[depth] = rem;
        int sz = depth + 1;

        /* Determine frequencies of each factor; the list is sorted */
        seq_t max1 = 0, max2 = 0;
        idx_t cur = fac[0];
        int curcnt = 1;

        for (int i = 1; i < sz; ++i) {
            if (fac[i] == cur) {
                curcnt += 1;
            } else {
                if (curcnt > max1) {
                    max2 = max1;
                    max1 = curcnt;
                } else if (curcnt > max2) {
                    max2 = curcnt;
                }
                cur = fac[i];
                curcnt = 1;
            }
        }

        /* Final block */
        if (curcnt > max1) {
            max2 = max1;
            max1 = curcnt;
        } else if (curcnt > max2) {
            max2 = curcnt;
        }

        /* Unique mode if the highest frequency is unambiguous */
        if (max1 > max2) {
            cnt += 1;
        }
    }

    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Workspace for current factor list; depth will never exceed 64 for n <= 10000 */
    static idx_t fac[64];
    return count_factorizations((idx_t)n, (idx_t)2, fac, 0);
}
