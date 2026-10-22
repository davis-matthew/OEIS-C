/*
 * A322285  Triangle read by rows: T(n,k) is the Damerau-Levenshtein distance
 *          between n and k in binary representation, 0 <= k <= n.
 * https://oeis.org/A322285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = Damerau–Levenshtein distance between the binary
 *            representations of r and k where n = r(r+1)/2 + k, 0 ≤ k ≤ r
 *            (triangle read by rows)
 * Range:     u8 n <= 9829, u16 n <= 9829, u32 n <= 9829, u64 n <= 9829,
 *            u128 n <= 9829, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* number of bits needed to represent x, with 0 having length 1 */
static int bitlen(idx_t x)
{
    if (x == 0) return 1;
    int len = 0;
    while (x) { len++; x >>= 1; }
    return len;
}

/* fill buf[0..len-1] with the bits of x (most‐significant bit first) */
static void bits_of(idx_t x, int *buf, int *len)
{
    *len = bitlen(x);
    for (int i = 0; i < *len; ++i) {
        int shift = *len - 1 - i;
        buf[i] = (x >> shift) & 1;
    }
}

/* Damerau–Levenshtein distance between binary strings of a and b */
static seq_t dl_distance(idx_t a, idx_t b)
{
    int A[65], B[65];
    int lenA, lenB;
    bits_of(a, A, &lenA);
    bits_of(b, B, &lenB);

    /* DP table of size (lenA+1) × (lenB+1) */
    int d[66][66] = {0};

    for (int i = 0; i <= lenA; ++i) d[i][0] = i;
    for (int j = 0; j <= lenB; ++j) d[0][j] = j;

    for (int i = 1; i <= lenA; ++i) {
        for (int j = 1; j <= lenB; ++j) {
            int cost = (A[i-1] == B[j-1]) ? 0 : 1;
            int best = d[i-1][j] + 1;               /* deletion */
            if (d[i][j-1] + 1 < best) best = d[i][j-1] + 1; /* insertion */
            int sub = d[i-1][j-1] + cost;           /* substitution/match */
            if (sub < best) best = sub;
            if (i > 1 && j > 1 && A[i-1] == B[j-2] &&
                A[i-2] == B[j-1]) {
                int trans = d[i-2][j-2] + 1;        /* adjacent transposition */
                if (trans < best) best = trans;
            }
            d[i][j] = best;
        }
    }

    return (seq_t)d[lenA][lenB];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((((unsigned __int128)(r + 1) * (r + 2)) / 2) <= n) {
        ++r;
    }
    idx_t offset = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - offset;
    return dl_distance(r, k);
}
