/*
 * A401022  Largest 3-smooth divisor of n*(n+1).
 * https://oeis.org/A401022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = A065331(n) * A065331(n+1) = 2^(v2(n)+v2(n+1)) *
 *            3^(v3(n)+v3(n+1))
 * Range:     u8 n <= 62, u16 n <= 6559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_v2, count_v3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_v2.h"
#include "../utils/count_v3.h"

seq_t a(idx_t n) {
    uint64_t n_val = (uint64_t)n;
    uint64_t pn = n_val + 1ULL;          /* n+1 */

    int e2_n  = count_v2(n_val);
    int e3_n  = count_v3(n_val);
    int e2_np1 = count_v2(pn);
    int e3_np1 = count_v3(pn);

    int exp2 = e2_n + e2_np1;
    int exp3 = e3_n + e3_np1;

    seq_t result = 1;
    result <<= exp2;                     /* multiply by 2^exp2 */
    for (int i = 0; i < exp3; ++i) {
        result *= 3;                     /* multiply by 3^exp3 */
    }

    return result;
}
