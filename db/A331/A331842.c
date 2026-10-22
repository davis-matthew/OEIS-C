/*
 * A331842  Number of positive integer solutions (x,y) to the equation x^y =
 *          y^(nx).
 * https://oeis.org/A331842
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integer solutions (x,y) of x^y = y^(n
 *            x)
 * Range:     u8 n <= 9611, u16 n <= 9611, u32 n <= 9611, u64 n <= 9611,
 *            u128 n <= 9611, big any n
 * Helpers:   is_kth_power, is_power_of_integer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_kth_power.h"
#include "../utils/is_power_of_integer.h"

/* integer square root, works for the small values we need */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* behaviour below offset is irrelevant */

    seq_t count = 1;                  /* (1,1) */

    for (idx_t k = 1; ; ++k) {
        uint64_t two_pow = 1ULL << k;
        uint64_t bound   = (uint64_t)n * (1 + k);
        if (two_pow > bound)
            break;                    /* no more k can give a²ⁿ ≤ n(1+k) */

        idx_t nk = n * k;
        idx_t sq = isqrt_u64(nk);
        idx_t d;
        for (d = 1; d <= sq; ++d) {
            if (nk % d == 0) {
                /* first divisor t = d */
                idx_t t = d;
                uint64_t rhs = (uint64_t)n + (uint64_t)t;
                uint64_t a_val;
                if (is_kth_power(rhs, k, &a_val)) {
                    if (!is_power_of_integer(a_val))
                        count++;
                }

                /* second divisor t = nk/d if distinct */
                idx_t d2 = nk / d;
                if (d2 != d) {
                    t = d2;
                    rhs = (uint64_t)n + (uint64_t)t;
                    if (is_kth_power(rhs, k, &a_val)) {
                        if (!is_power_of_integer(a_val))
                            count++;
                    }
                }
            }
        }
    }
    return count;
}
