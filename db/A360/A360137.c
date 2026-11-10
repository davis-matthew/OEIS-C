/*
 * A360137  a(n) = V(A026430(n)), where V(1) = 1 and V(k) = A285953(k+1) for k
 *          >= 2.
 * https://oeis.org/A360137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n >= 2, a(n) = A285953(A026430(n) - 1), where
 *            A285953(m) = 3*m - 1 + parity(popcount(2*m-1)) and A026430(n) =
 *            n + #{k < n : Thue-Morse t(k) = 1}
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t a_a(idx_t n)
{
    /* Sum of first n terms of Thue-Morse+1 (A026430). */
    uint64_t s;
    if ((n & 1u) == 0) {
        s = (uint64_t)n + (uint64_t)(n >> 1);
    } else {
        s = (uint64_t)n + (uint64_t)((n - 1) >> 1) + (uint64_t)__builtin_parityll((uint64_t)(n - 1));
    }
    return (seq_t)s;
}

static seq_t a_b(seq_t m)
{
    /* A285953(m) = 3m - 1 + parity(popcount(2m-1)); m is small here. */
    uint64_t mm = (uint64_t)m;
    uint64_t low = (mm << 1) - 1ULL;
    return (seq_t)3 * m - (seq_t)1 + (seq_t)__builtin_parityll(low);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t k = a_a(n);
    return a_b(k - (seq_t)1);
}
