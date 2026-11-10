/*
 * A266635  Permutation of natural numbers: a(n) =
 *          A126760(A264965(A007310(n))).
 * https://oeis.org/A266635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A126760(A264965(A007310(n))), with A007310(n) = 3n-1-(n
 *            mod 2), A264965(k) = adjusted base-3 reversal of adjusted base-2
 *            reversal of k
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9624 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A264965_rev2(seq_t n)
{
    if (n == (seq_t)0) return (seq_t)0;
    seq_t x = n;
    unsigned t = 0;
    while ((x % (seq_t)2) == (seq_t)0) { x /= (seq_t)2; ++t; }
    seq_t rev = (seq_t)0;
    seq_t y = x;
    while (y > (seq_t)0) {
        rev = rev * (seq_t)2 + (y % (seq_t)2);
        y /= (seq_t)2;
    }
    for (unsigned i = 0; i < t; ++i) rev *= (seq_t)2;
    return rev;
}

static seq_t A264965_rev3(seq_t n)
{
    if (n == (seq_t)0) return (seq_t)0;
    seq_t x = n;
    unsigned t = 0;
    while ((x % (seq_t)3) == (seq_t)0) { x /= (seq_t)3; ++t; }
    seq_t rev = (seq_t)0;
    seq_t y = x;
    while (y > (seq_t)0) {
        rev = rev * (seq_t)3 + (y % (seq_t)3);
        y /= (seq_t)3;
    }
    for (unsigned i = 0; i < t; ++i) rev *= (seq_t)3;
    return rev;
}

static seq_t A126760_a(seq_t m)
{
    seq_t result = 0;
    while (m > (seq_t)0) {
        seq_t r = m % (seq_t)6;
        if (r == (seq_t)1) {
            result = (seq_t)2 * ((m - (seq_t)1) / (seq_t)6) + (seq_t)1;
            break;
        }
        if (r == (seq_t)5) {
            result = (seq_t)2 * ((m - (seq_t)5) / (seq_t)6) + (seq_t)2;
            break;
        }
        if ((m % (seq_t)2) == (seq_t)0) { m /= (seq_t)2; continue; }
        if ((m % (seq_t)3) == (seq_t)0) { m /= (seq_t)3; continue; }
        break;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t m = (seq_t)3 * sn - (seq_t)1 - (sn & (seq_t)1);
    seq_t b = A264965_rev3(A264965_rev2(m));
    return A126760_a(b);
}
