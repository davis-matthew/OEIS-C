/*
 * A379013  Positions of zeros in A378505.
 * https://oeis.org/A379013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; if z = a(n-1) then a(n) is the smallest divisor k of
 *            z*(z+1) with k >= z+2 such that k is odd, or k is even and
 *            z*(z+1)/k is odd (a(2) = 3). Between zeros, A378505(k-1) =
 *            ((k-1)*k - z*(z+1))/2, so k divides it exactly when this
 *            condition holds (Corneth's method)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t A379013_terms[256];
static size_t A379013_cnt = 0;

static size_t A379013_factor(uint64_t m, uint64_t *pr, int *ex, size_t c)
{
    uint64_t p;
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) { m /= 2; e++; }
        pr[c] = 2; ex[c] = e; c++;
    }
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            pr[c] = p; ex[c] = e; c++;
        }
    }
    if (m > 1) { pr[c] = m; ex[c] = 1; c++; }
    return c;
}

static int A379013_next(uint64_t z, uint64_t *out)
{
    uint64_t pr[64];
    int ex[64];
    size_t c = 0, i, nd, cap;
    unsigned __int128 T, best = 0, lo;
    unsigned __int128 *divs;
    int found = 0;

    if (z == 0) { *out = 3; return 1; }
    c = A379013_factor(z, pr, ex, c);
    c = A379013_factor(z + 1, pr, ex, c);
    T = (unsigned __int128)z * (unsigned __int128)(z + 1);
    lo = (unsigned __int128)z + 2;

    cap = 1;
    for (i = 0; i < c; i++) cap *= (size_t)(ex[i] + 1);
    divs = (unsigned __int128 *)malloc(cap * sizeof(unsigned __int128));
    if (!divs) return 0;
    divs[0] = 1; nd = 1;
    for (i = 0; i < c; i++) {
        size_t base = nd, j;
        int e;
        unsigned __int128 pw = 1;
        for (e = 1; e <= ex[i]; e++) {
            pw *= pr[i];
            for (j = 0; j < base; j++) divs[nd++] = divs[j] * pw;
        }
    }
    for (i = 0; i < nd; i++) {
        unsigned __int128 k = divs[i];
        if (k < lo) continue;
        if ((k & 1) == 0 && ((T / k) & 1) == 0) continue;
        if (!found || k < best) { best = k; found = 1; }
    }
    free(divs);
    if (!found || best > (unsigned __int128)UINT64_MAX) return 0;
    *out = (uint64_t)best;
    return 1;
}

seq_t a(idx_t n)
{
    if (A379013_cnt == 0) A379013_terms[A379013_cnt++] = 0;
    while (A379013_cnt < (size_t)n && A379013_cnt < 256) {
        uint64_t nx;
        if (!A379013_next(A379013_terms[A379013_cnt - 1], &nx)) break;
        A379013_terms[A379013_cnt++] = nx;
    }
    if ((size_t)n > A379013_cnt) return (seq_t)0;
    return (seq_t)A379013_terms[n - 1];
}
