/*
 * A335138  a(n) = 1 + Sum_{k=1..2*n}
 *          sign((sign(n+Sum_{j=2..k}-|A309229(n,j)|)+1)).
 * https://oeis.org/A335138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..2n} sign(sign(n - Sum_{j=2..k}
 *            |T(n,j)|)+1), T = A309229, i.e. 1 + #{k in 1..2n : Sum_{j=2..k}
 *            |T(n,j)| <= n}. T(1,k)=1, T(n,1)=n, T(n,k)=T(n-k,k) if n>k, else
 *            Sum_{i=0..n-1} T(n-1,k-i) - Sum_{i=1..n-1} T(n,k-i)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    long long N = (long long)n;
    long long W = 2 * N + 1;
    long long *T = (long long *)calloc((size_t)((N + 1) * W), sizeof(long long));
    long long r, c, i;
    for (r = 1; r <= N; r++) {
        for (c = 1; c <= 2 * N; c++) {
            long long v;
            if (r == 1) v = 1;
            else if (c == 1) v = r;
            else if (r > c) v = T[(r - c) * W + c];
            else {
                long long s1 = 0, s2 = 0;
                for (i = 0; i < r; i++) s1 += T[(r - 1) * W + (c - i)];
                for (i = 1; i < r; i++) s2 += T[r * W + (c - i)];
                v = s1 - s2;
            }
            T[r * W + c] = v;
        }
    }
    long long cnt = 1; /* k = 1: empty sum */
    long long sum = 0;
    for (c = 2; c <= 2 * N; c++) {
        long long v = T[N * W + c];
        sum += (v < 0 ? -v : v);
        if (sum <= N) cnt++;
        else break;
    }
    free(T);
    return (seq_t)(cnt + 1);
}
