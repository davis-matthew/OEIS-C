/*
 * A316838  Indices of 1's in A254990.
 * https://oeis.org/A316838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position (0-indexed) of the n-th 1 in the 4-bonacci word
 *            A254990, the fixed point of 0->01, 1->02, 2->03, 3->0. Found by
 *            descending S(k) = S(k-1)S(k-2)S(k-3)S(k-4), with lengths and
 *            1-counts cached
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8704 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A316838_u128;

static const uint8_t A316838_small[4][8] = {
    {0},
    {0, 1},
    {0, 1, 0, 2},
    {0, 1, 0, 2, 0, 1, 0, 3}
};
static const int A316838_slen[4] = {1, 2, 4, 8};

seq_t a(idx_t n)
{
    A316838_u128 L[140], C[140];
    int k, i;
    L[0] = 1; L[1] = 2; L[2] = 4; L[3] = 8;
    C[0] = 0; C[1] = 1; C[2] = 1; C[3] = 2;
    A316838_u128 t = (A316838_u128)n;
    k = 3;
    while (C[k] < t && k < 135) {
        k++;
        L[k] = L[k-1] + L[k-2] + L[k-3] + L[k-4];
        C[k] = C[k-1] + C[k-2] + C[k-3] + C[k-4];
    }
    /* smallest k with C[k] >= t */
    while (k > 0 && C[k-1] >= t) k--;
    A316838_u128 off = 0;
    while (k > 3) {
        int j;
        for (j = 1; j <= 4; j++) {
            int p = k - j;
            if (t <= C[p]) { k = p; break; }
            t -= C[p];
            off += L[p];
        }
    }
    {
        int cnt = 0;
        for (i = 0; i < A316838_slen[k]; i++) {
            if (A316838_small[k][i] == 1) {
                cnt++;
                if ((A316838_u128)cnt == t) {
                    off += (A316838_u128)i;
                    break;
                }
            }
        }
    }
    return (seq_t)off;
}
