/*
 * A255741  Square array read by antidiagonals upwards: T(n,k), n>=1, k>=1, in
 *          which row n lists the partial sums of the n-th row of the square
 *          array of A255740.
 * https://oeis.org/A255741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,1)=1; T(n,k) = 1 + Sum_{j=1..k-1} (n-1)*(n-2)^(A000120(j)-1)
 *            for n>=2 (partial sums of rows of A255740); T(1,k)=1; read by
 *            antidiagonals upwards
 * Range:     u8 n <= 98, u16 n <= 511, u32 n <= 5523, u64 n <= 11325,
 *            u128 n <= 11325, big any n
 * Validated: standard, pass, 107 terms through n = 7778 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t d = 1, rem = n;
    while (rem > d) { rem -= d; d++; }
    idx_t k = rem;          /* column */
    idx_t r = d + 1 - rem;  /* row */
    if (r == 1 || k == 1) return (seq_t)1;

    /* count j in 1..k-1 by popcount */
    idx_t cnt[65];
    for (int i = 0; i < 65; i++) cnt[i] = 0;
    for (idx_t j = 1; j < k; j++)
        cnt[__builtin_popcountll((unsigned long long)j)]++;

    seq_t res = 1;
    seq_t base = (seq_t)r - 1;
    seq_t mult = (seq_t)r - 2;
    seq_t pw = 1; /* mult^(p-1) */
    for (int p = 1; p < 65; p++) {
        if (p > 1) pw = pw * mult;
        if (cnt[p]) res += (seq_t)cnt[p] * base * pw;
    }
    return res;
}
