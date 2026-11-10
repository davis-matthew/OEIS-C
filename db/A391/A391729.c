/*
 * A391729  a(n) = Sum_{k=1..A003817(n)} [A048720(A065621(n),k) == n*k], where
 *          [ ] is the Iverson bracket.
 * https://oeis.org/A391729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..A003817(n)} [A048720(A065621(n),k) == n*k];
 *            computed by direct count
 * Range:     u8 n <= 16, u16 n <= 580, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 115 terms through n = 9499 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A065621_a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    idx_t low = n & (idx_t)(-(idx_t)n);
    seq_t low_t = (seq_t)low;
    return n_t ^ ((n_t - low_t) << 1);
}

static seq_t A048720_gf2_mul(seq_t i, idx_t j)
{
    seq_t result = 0;
    seq_t shifted = i;
    while (j) {
        if (j & 1)
            result ^= shifted;
        shifted <<= 1;
        j >>= 1;
    }
    return result;
}

static idx_t A003817_idx(idx_t n)
{
    idx_t r = 0;
    while (n) {
        r = (r << 1) | 1;
        n >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t N = A003817_idx(n);
    seq_t m = A065621_a(n);
    seq_t cnt = 0;
    for (idx_t k = 1; k <= N; k++) {
        seq_t lhs = A048720_gf2_mul(m, k);
        seq_t rhs = (seq_t)n * (seq_t)k;
        if (lhs == rhs)
            cnt++;
    }
    return cnt;
}
