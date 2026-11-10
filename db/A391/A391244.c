/*
 * A391244  Triangular table T(n,k) = A048720(n, A065621(k)), n >= 1, 1 <= k
 *          <= n, read by rows.
 * https://oeis.org/A391244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A048720(n, A065621(k)), 1<=k<=n, read by rows;
 *            A065621(k) = k XOR ((k - lowbit(k)) << 1); A048720 is carryless
 *            GF(2) multiplication
 * Range:     u8 n <= 128, u16 n <= 10440, u32 n <= 10440, u64 n <= 10440,
 *            u128 n <= 10440, big any n
 * Validated: standard, pass, 94 terms through n = 9959 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A065621_a(idx_t k)
{
    seq_t k_t = (seq_t)k;
    idx_t low = k & (~k + 1);
    seq_t low_t = (seq_t)low;
    return k_t ^ ((k_t - low_t) << 1);
}

static seq_t A048720_mul(idx_t i, seq_t j)
{
    seq_t result = 0;
    seq_t shifted = j;
    while (i) {
        if (i & 1)
            result ^= shifted;
        shifted <<= 1;
        i >>= 1;
    }
    return result;
}

seq_t a(idx_t m)
{
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < m)
        r++;
    idx_t k = m - (r * (r - 1)) / 2;
    return A048720_mul(r, A065621_a(k));
}
