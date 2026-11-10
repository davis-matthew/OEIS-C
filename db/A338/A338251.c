/*
 * A338251  Nonnegative values in A317050, in order of appearance.
 * https://oeis.org/A338251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(0) = 0; a(n) = A317050(A053738(n)) for n > 0, where A053738(n)
 *            is the n-th number with an odd number of binary digits, and
 *            A317050(m) is the Gray code of m read as a negabinary number
 * Range:     u8 n <= 213, u16 n <= 5461, u32 n <= 5461, u64 n <= 5461,
 *            u128 n <= 5461, big any n
 * Validated: standard, pass, 84 terms through n = 5452 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A053738_a(idx_t n)
{
    idx_t cum = 0;
    unsigned L = 1;
    while (cum + ((idx_t)1 << (L - 1)) < n) {
        cum += ((idx_t)1 << (L - 1));
        L += 2;
    }
    return ((idx_t)1 << (L - 1)) + (n - cum - 1);
}

static seq_t A317050_a(idx_t m)
{
    idx_t gray = m ^ (m >> 1);
    seq_t pos = 0, neg = 0;
    seq_t base = 1;
    unsigned i = 0;
    while (gray) {
        if (gray & 1) {
            if (i & 1) neg += base;
            else pos += base;
        }
        base *= 2;
        gray >>= 1;
        i++;
    }
    return pos - neg;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return A317050_a(A053738_a(n));
}
