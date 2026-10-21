/*
 * A286145  Square array read by antidiagonals: A(n,k) = T(n XOR k, k), where
 *          T(n,k) is sequence A001477 considered as a two-dimensional table,
 *          and XOR is bitwise-xor (A003987).
 * https://oeis.org/A286145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = T(n XOR k, k) where T(n,k) is the two‑dimensional table
 *            from sequence A001477. For n ≥ 1, T(n,k) = n(n+3)/2 + k·n +
 *            k(k+1)/2. For n = 0, T(0,k) = k(k+1)/2. The coordinates (i,j)
 *            corresponding to index n are obtained by reading the array along
 *            antidiagonals
 * Range:     u8 n <= 65, u16 n <= 9706, u32 n <= 9706, u64 n <= 9706,
 *            u128 n <= 9706, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline seq_t T_func(idx_t n, idx_t k)
{
    if (n == 0)
    {
        /* T(0,k) = k(k+1)/2 */
        return (seq_t)k * (k + 1) / 2;
    }
    /* n ≥ 1:
       T(n,k) = n(n+3)/2 + k·n + k(k+1)/2
    */
    seq_t nn = (seq_t)n;
    seq_t kk = (seq_t)k;
    return nn * (nn + 3) / 2 + kk * nn + kk * (kk + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal index s such that
       s(s+1)/2 ≤ n < (s+1)(s+2)/2   */
    idx_t s = (idx_t)((sqrt((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);

    /* Correct possible rounding errors */
    while (((__uint128_t)s * (s + 1)) / 2 > n)
        --s;
    while (((__uint128_t)(s + 1) * (s + 2)) / 2 <= n)
        ++s;

    /* i, j for this index */
    __uint128_t before = (__uint128_t)s * (s + 1) / 2;
    idx_t offset = (idx_t)(n - before);
    idx_t i = offset;
    idx_t j = s - offset;

    idx_t nxor = i ^ j;
    return T_func(nxor, j);
}
