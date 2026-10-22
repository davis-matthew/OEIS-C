/*
 * A366940  a(n) is the number of positive squares with n digits, all
 *          distinct.
 * https://oeis.org/A366940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   _floor_isqrt_u64, distinct_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of positive n‑digit squares with all distinct digits.
   For n > 10 the value is zero because a decimal number cannot have more
   than ten distinct digits. */
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"   /* for _floor_isqrt_u64 */
#include "../utils/distinct_digits_uint64.h"

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t cnt[11];   /* indices 1..10 */

    if (!initialized) {
        uint64_t pow10[11];
        pow10[0] = 1ULL;
        for (idx_t i = 1; i <= 10; ++i)
            pow10[i] = pow10[i - 1] * 10ULL;

        for (idx_t d = 1; d <= 10; ++d) {
            uint64_t lower = pow10[d - 1];
            uint64_t upper = pow10[d] - 1ULL;

            uint64_t start = _floor_isqrt_u64(lower);
            if (start * start < lower)
                ++start;
            uint64_t end   = _floor_isqrt_u64(upper);

            seq_t c = 0;
            for (uint64_t m = start; m <= end; ++m) {
                uint64_t sq = m * m;
                if (distinct_digits_uint64(sq))
                    ++c;
            }
            cnt[d] = c;
        }
        cnt[0] = 0;
        initialized = 1;
    }

    if (n > 10)
        return 0;
    return cnt[(idx_t)n];
}
