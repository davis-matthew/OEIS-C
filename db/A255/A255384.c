/*
 * A255384  a(n) = sopfr(n)^2 - 2n, where sopfr(n) is the sum of the prime
 *          factors of n with multiplicity.
 * https://oeis.org/A255384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sopfr(n)^2 - 2*n
 * Range:     i8 n <= 12, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sopfr(n) = sum of prime factors of n with multiplicity */

#include <stdint.h>

static seq_t sopfr_u64(uint64_t val)
{
    seq_t sum = 0;
    uint64_t m = val;
    if (m <= 1) return 0;
    /* factor out twos */
    while ((m & 1ULL) == 0) {
        sum += (seq_t)2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) {
            sum += (seq_t)p;
            m /= p;
        }
    }
    if (m > 1) sum += (seq_t)m;   /* remaining prime factor */
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sum = sopfr_u64((uint64_t)n);
    seq_t res = sum * sum - (seq_t)2 * (seq_t)n;
    return res;
}
