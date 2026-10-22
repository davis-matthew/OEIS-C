/*
 * A307613  Inverse of the permutation A307485: one odd, two even, four odd,
 *          eight even, etc; extended with a(0) = 0.
 * https://oeis.org/A307613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * For n > 0: * let r = (n-1)/2 if n is odd, else (n-2)/2. * Set
 *            size = 1 (odd) or 2 (even) and repeatedly multiply by 4 * until
 *            r < size. Let pow4 be the corresponding power of 4. * Then for
 *            odd n : a(n) = 1 + (pow4 - 1) + r. * For even n: a(n) = 1 +
 *            (2*pow4 - 1) + r. * For n = 0: a(0) = 0
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    bool is_odd = (n & 1);
    idx_t rem = is_odd ? (n - 1) / 2 : (n - 2) / 2;
    idx_t size = is_odd ? 1 : 2;
    idx_t pow4 = 1;          // 4^m
    while (rem >= size) {
        rem -= size;
        size <<= 2;          // multiply by 4
        pow4 <<= 2;          // multiply by 4
    }
    idx_t total_before = is_odd ? (pow4 - 1) : (2 * pow4 - 1);
    idx_t idx_in_perm = 1 + total_before + rem;
    return (seq_t)idx_in_perm;
}
