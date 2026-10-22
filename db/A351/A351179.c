/*
 * A351179  Least positive integer m such that m^6*n = w^6 + x^3 + y^3 + z^3
 *          for some nonnegative integers w,x,y,z.
 * https://oeis.org/A351179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive integer m such that m^6 * n = w^6 +
 *            x^3 + y^3 + z^3 for some non‑negative integers w,x,y,z. The
 *            function enumerates increasing m and tests the existence of such
 *            a representation by a two‑pointer search over cubes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   icbrt_u128, pow3_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/icbrt_u128.h"
#include "../utils/pow3_u128.h"

/* 6th power, result in 128 bits. */
static inline unsigned __int128 pow6_u128(unsigned long long a)
{
    unsigned __int128 r = 1;
    for (int i = 0; i < 6; ++i) r *= a;
    return r;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;          /* a(0) = 1 */

    for (idx_t m = 1; ; ++m) {                   /* try increasing m   */
        unsigned __int128 m6 = pow6_u128((unsigned long long)m);
        unsigned __int128 target = m6 * n;

        /* iterate over w^6 <= target   */
        for (unsigned long long w = 0; ; ++w) {
            unsigned __int128 w6 = pow6_u128(w);
            if (w6 > target) break;

            unsigned __int128 rem = target - w6;          /* remaining sum */
            unsigned long long max_z = icbrt_u128(rem);

            /* iterate over z^3 <= rem   */
            for (unsigned long long z = 0; z <= max_z; ++z) {
                unsigned __int128 z3 = pow3_u128(z);
                unsigned __int128 rem2 = rem - z3;

                /* two‑pointer search for x^3 + y^3 = rem2   */
                unsigned long long low = 0;
                unsigned long long high = icbrt_u128(rem2);
                while (low <= high) {
                    unsigned __int128 low3 = pow3_u128(low);
                    unsigned __int128 high3 = pow3_u128(high);
                    unsigned __int128 sum = low3 + high3;

                    if (sum == rem2) return (seq_t)m;   /* found representation */
                    if (sum < rem2) low++;              /* need larger sum   */
                    else high--;                        /* need smaller sum */
                }
            }
        }
    }
    return (seq_t)0;          /* unreachable, safety fallback */
}
