/*
 * A340285  Number of partitions of n into 3 parts such that the largest part
 *          is relatively prime to each of the other two parts.
 * https://oeis.org/A340285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/3)} Sum_{i=k..floor((n-k)/2)}
 *            floor(1/gcd(n-i-k,k)) * floor(1/gcd(n-i-k,i))
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"


seq_t a(idx_t n) {
    if (n < 3) return 0;
    seq_t count = 0;
    for (idx_t k = 1; k <= n / 3; ++k) {
        idx_t max_i = (n - k) / 2;
        for (idx_t i = k; i <= max_i; ++i) {
            idx_t third = n - i - k;
            seq_t g1 = gcd((seq_t)third, (seq_t)k);
            if (g1 != 1) continue;
            seq_t g2 = gcd((seq_t)third, (seq_t)i);
            if (g2 != 1) continue;
            ++count;
        }
    }
    return count;
}
