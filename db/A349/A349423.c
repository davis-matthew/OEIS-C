/*
 * A349423  Index of first occurrence of n in A348179, or -1 if n never
 *          appears.
 * https://oeis.org/A349423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = least k with A348179(k) = n (search: backtracking over
 *            the digits of k, for each length L >= digits(n), leading zeros
 *            of the image allowed), or -1 if none is found
 * Range:     i8 n <= 9, i16 n <= 22, i32 n <= 1533, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9922 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int A349423_L;
static int A349423_out[40];
static int A349423_k[40];
static long long A349423_nodes;

static int A349423_dfs(int i)
{
    int L = A349423_L;
    if (i == L) return 1;
    if (++A349423_nodes > 1000000LL) return 0;
    int forced = A349423_k[i];
    int lo = 0, hi = 9;
    if (forced >= 0) { lo = hi = forced; }
    if (i == 0 && L > 1 && lo == 0) lo = 1;
    for (int d = lo; d <= hi; d++) {
        int j = (i + d) % L;
        int need = A349423_out[i];
        int set_i = 0, set_j = 0;
        if (forced < 0) { A349423_k[i] = d; set_i = 1; }
        if (A349423_k[j] >= 0) {
            if (A349423_k[j] != need) {
                if (set_i) A349423_k[i] = -1;
                continue;
            }
        } else {
            A349423_k[j] = need; set_j = 1;
        }
        if (A349423_dfs(i + 1)) return 1;
        if (set_j) A349423_k[j] = -1;
        if (set_i) A349423_k[i] = -1;
        if (A349423_nodes > 1000000LL) return 0;
    }
    return 0;
}

seq_t a(idx_t n)
{
    int dg[24];
    int len = 0;
    idx_t t = n;
    if (t == 0) { dg[len++] = 0; }
    while (t > 0) { dg[len++] = (int)(t % 10); t /= 10; }
    /* dg[0] is least significant digit */
    int maxL = 18;
    if (len > maxL) maxL = len;
    for (int L = len; L <= maxL; L++) {
        A349423_L = L;
        for (int i = 0; i < L; i++) {
            int pos = L - 1 - i; /* digit index from least significant */
            A349423_out[i] = (pos < len) ? dg[pos] : 0;
            A349423_k[i] = -1;
        }
        A349423_nodes = 0;
        if (A349423_dfs(0)) {
            seq_t r = 0;
            for (int i = 0; i < L; i++) r = r * 10 + (seq_t)A349423_k[i];
            return r;
        }
    }
    return (seq_t)-1;
}
