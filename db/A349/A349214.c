/*
 * A349214  a(n) = Sum_{k=1..n} k^c(k), where c is the prime characteristic
 *          (A010051).
 * https://oeis.org/A349214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} (k is prime ? k : 1)
 * Range:     u8 n <= 40, u16 n <= 918, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        bool isprime = false;
        if (k > 1) {
            isprime = true;
            for (idx_t d = 2; d * d <= k; ++d) {
                if (k % d == 0) {
                    isprime = false;
                    break;
                }
            }
        }
        if (isprime)
            sum += (seq_t)k;
        else
            sum += 1;
    }
    return sum;
}
