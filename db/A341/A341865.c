/*
 * A341865  The cardinality of the largest multiset of positive integers whose
 *          product and sum equals n.
 * https://oeis.org/A341865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - Sum_{d_i*(p_i-1)}, where n = ∏ p_i^d_i
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t temp = nn;
    unsigned long long sum = 0;

    /* factor 2 */
    if (temp % 2 == 0) {
        unsigned long long cnt = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            cnt++;
        }
        sum += cnt * (2ULL - 1);
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned long long cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                cnt++;
            }
            sum += cnt * (p - 1);
        }
    }

    if (temp > 1) { /* remaining prime factor */
        sum += (unsigned long long)1 * (temp - 1);
    }

    unsigned long long result = nn - sum;
    return (seq_t)result;
}
