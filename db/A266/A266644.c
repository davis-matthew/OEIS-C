/*
 * A266644  Permutation of nonnegative integers: a(n) = A264966(3*n) / 3.
 * https://oeis.org/A266644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A246200(A263273(n)), where A263273 is the bijective
 *            base-3 reverse and A246200(m) = A057889(3*m)/3
 * Range:     u8 n <= 87, u16 n <= 19682, u32 n <= 19682, u64 n <= 19682,
 *            u128 n <= 19682, big any n
 * Validated: standard, pass, 98 terms through n = 9541 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A263273_a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t temp = n;
    seq_t p3 = (seq_t)1;
    while ((temp % (idx_t)3) == 0) {
        temp /= (idx_t)3;
        p3 *= (seq_t)3;
    }
    seq_t rev = (seq_t)0;
    idx_t m = temp;
    while (m > 0) {
        rev = rev * (seq_t)3 + (seq_t)(m % (idx_t)3);
        m /= (idx_t)3;
    }
    return rev * p3;
}

static seq_t A246200_of(seq_t x)
{
    seq_t m = x * 3;
    if (m == 0)
        return 0;

    idx_t len = 0;
    seq_t tmp = m;
    while (tmp != 0) {
        len++;
        tmp >>= 1;
    }

    seq_t rev = 0;
    tmp = m;
    for (idx_t i = 0; i < len; i++) {
        rev = (rev << 1) | (tmp & 1);
        tmp >>= 1;
    }

    idx_t t = 0;
    tmp = m;
    while ((tmp & 1) == 0) {
        t++;
        tmp >>= 1;
    }

    rev <<= t;
    return rev / 3;
}

seq_t a(idx_t n)
{
    return A246200_of(A263273_a(n));
}
