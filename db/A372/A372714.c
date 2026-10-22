/*
 * A372714  a(n) = tau(3*n-1) = A000005(3*n-1).
 * https://oeis.org/A372714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(3*n-1) (tau = divisor count)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t tau_uint64(uint64_t m) {
    seq_t result = (seq_t)1;
    seq_t exp;
    uint64_t p;

    for (p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            result *= (exp + 1);
        }
    }
    if (m > 1) {
        /* Remaining prime factor */
        result *= 2;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) {
        /* Undefined (offset 1), but return 0 for safety */
        return (seq_t)0;
    }
    uint64_t m = 3 * (uint64_t)n - 1;
    return tau_uint64(m);
}
