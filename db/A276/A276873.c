/*
 * A276873  Sums-complement of the Beatty sequence for sqrt(7).
 * https://oeis.org/A276873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer that cannot be written as ⌊k·√7⌋
 *            or ⌊k·√7⌋+1 for any integer k ≥ 1. (Offset 1)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned long long: returns floor(sqrt(x)) */
static seq_t int_sqrt_u64(unsigned long long x) {
    unsigned long long low = 0, high = 0xFFFFFFFFULL; /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        unsigned long long mid = (low + high) >> 1;
        unsigned long long product = mid * mid;
        if (product == x) return (seq_t)mid;
        if (product < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (seq_t)high; /* floor sqrt */
}

/* Main sequence function */
seq_t a(idx_t n) {
    idx_t found = 0;        /* number of complementary terms found */
    seq_t m = 1;            /* current integer we are testing */
    idx_t k = 1;            /* k for ⌊k·√7⌋ */
    seq_t val1 = (seq_t)int_sqrt_u64(7ULL * k * k); /* ⌊k·√7⌋ */

    while (1) {
        /* Move k until the interval [val1, val1+1] contains or exceeds m */
        while ((val1 + 1) < m) {
            ++k;
            val1 = (seq_t)int_sqrt_u64(7ULL * k * k);
        }

        /* Check if m is representable as ⌊k·√7⌋ or ⌊k·√7⌋+1 */
        if (m != val1 && m != val1 + 1) {
            ++found;
            if (found == n)
                return m;     /* found the nth complementary term */
        }

        ++m;
    }
}
