/*
 * A278222  The least number with the same prime signature as A005940(n+1).
 * https://oeis.org/A278222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A046523(A005940(1+n)): take the exponents of A005940(n+1)
 *            from the binary digits of n, sort them in descending order, and
 *            return prod prime(j)^e_j
 * Range:     u8 n <= 170, u16 n <= 2778, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 115 terms through n = 9681 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned A278222_pr[64] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,
    283,293,307,311
};

seq_t a(idx_t n)
{
    int e[72];
    int i, j, k, cnt = 0;
    for (i = 0; i < 72; i++) e[i] = 0;
    idx_t m = n;
    int idx = 0;
    while (m > 0) {
        if (m & 1) {
            e[idx]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            idx++;
        }
    }
    /* collect nonzero exponents */
    int v[72];
    for (i = 0; i < 72; i++) if (e[i] > 0) v[cnt++] = e[i];
    /* sort descending (insertion sort) */
    for (i = 1; i < cnt; i++) {
        int x = v[i];
        j = i - 1;
        while (j >= 0 && v[j] < x) { v[j + 1] = v[j]; j--; }
        v[j + 1] = x;
    }
    seq_t result = 1;
    for (i = 0; i < cnt && i < 64; i++) {
        for (k = 0; k < v[i]; k++) result *= (seq_t)A278222_pr[i];
    }
    return result;
}
