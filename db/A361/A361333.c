/*
 * A361333  Index of prime(n) in A351495.
 * https://oeis.org/A361333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = index of prime(n) in A351495. For n ≤ 4 the sequence is
 *            2,3,6,23. For n ≥ 5, a(n) = 23 * ∏_{k=4}^{n-1} prime(k), where
 *            prime(k) is the k‑th prime (prime(1)=2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but guard */
    if (n <= 4) {
        static const seq_t init[4] = { 2, 3, 6, 23 };
        return init[n - 1];
    }
    seq_t result = 23;
    for (idx_t i = 4; i < n; ++i) {
        result *= get_prime(i);
    }
    return result;
}
