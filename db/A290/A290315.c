/*
 * A290315  Triangle T(n, k) read by rows: row n gives the coefficients of the
 *          numerator polynomials of the o.g.f. of the (n+1)-th diagonal of
 *          the Sheffer triangle A154537 (S2[2,1] generalized Stirling2), for
 *          n >= 0.
 * https://oeis.org/A290315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   P(n,x) = (1-2x)^(2n+1) * Sum_{m>=0} A154537(n+m,m) x^m truncated
 *            to degree n; T(n,k) = [x^k] P(n,x). A154537: T(N,0)=1,
 *            T(N,k)=2*T(N-1,k-1)+(2k+1)*T(N-1,k). Row n of this triangle is
 *            read by rows
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 40, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t R = 0;
    while ((R + 1) * (R + 2) / 2 <= n)
        ++R;
    idx_t k = n - R * (R + 1) / 2;

    size_t sz = (size_t)(2 * R + 2);
    seq_t *row = mem_new(seq_t, sz);
    seq_t *t = mem_new(seq_t, (size_t)(R + 1));

    row[0] = (seq_t)1;
    if (R == 0)
        t[0] = row[0];
    for (idx_t N = 1; N <= 2 * R; ++N) {
        for (idx_t j = N; j >= 1; --j)
            row[j] = (seq_t)2 * row[j - 1] + (seq_t)(2 * j + 1) * row[j];
        if (N >= R)
            t[N - R] = row[N - R];
    }

    seq_t pos = 0, neg = 0;
    seq_t c = 1, p2 = 1;
    for (idx_t j = 0; j <= k; ++j) {
        if (j > 0) {
            c = c * ((seq_t)(2 * R + 2) - (seq_t)j) / (seq_t)j;
            p2 = p2 * 2;
        }
        seq_t term = c * p2 * t[k - j];
        if (j & 1)
            neg += term;
        else
            pos += term;
    }
    seq_t res = pos - neg;

    mem_free(row);
    mem_free(t);
    return res;
}
