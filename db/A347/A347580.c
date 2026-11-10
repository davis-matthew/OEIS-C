/*
 * A347580  Triangle read by rows: T(n,k) is the number of chains of length k
 *          in the poset of all arithmetic progressions contained in {1,...,n}
 *          of length in the range [1..n-1], ordered by inclusion.
 * https://oeis.org/A347580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,1) = 1; T(n,k) = Sum_{l=1..n-1} c(n,l)*T(l,k-1) for 2<=k<=n,
 *            where c(n,1) = n and c(n,l) = A338993(n,l)/2 = n*f -
 *            (l-1)*f*(f+1)/2 with f = floor((n-1)/(l-1)) for l>=2 (the number
 *            of arithmetic progressions of length l in {1..n}); a(n) =
 *            T(r,k), r(r-1)/2 < n <= r(r+1)/2, k = n - r(r-1)/2
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A347580_cnt(long long n, long long l) {
    if (l == 1) return (seq_t)n;
    long long f = (n - 1) / (l - 1);
    long long sub = (l - 1) * (f * (f + 1) / 2);
    return (seq_t)(n * f - sub);
}

seq_t a(idx_t n) {
    long long r = 1;
    while ((unsigned long long)r * (unsigned long long)(r + 1) / 2 < (unsigned long long)n) r++;
    long long k = (long long)(n - (idx_t)((unsigned long long)r * (unsigned long long)(r - 1) / 2));
    long long w = r + 1;
    seq_t *W = mem_new(seq_t, (size_t)(w * w));
    for (long long L = 1; L <= r; L++) {
        W[L * w + 1] = (seq_t)1;
        for (long long j = 2; j <= k && j <= L; j++) {
            seq_t s = (seq_t)0;
            for (long long l = 1; l < L; l++) {
                seq_t p = W[l * w + (j - 1)];
                if (p == (seq_t)0) continue;
                s += A347580_cnt(L, l) * p;
            }
            W[L * w + j] = s;
        }
    }
    seq_t res = W[r * w + k];
    mem_free(W);
    return res;
}
