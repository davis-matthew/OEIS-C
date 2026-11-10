/*
 * A329624  Number of iterations of A329623 for starting value n before a
 *          repeated value appears, or -1 if this never happens.
 * https://oeis.org/A329624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = number of iterations of A329623 (m -> |m - concat of sums
 *            of adjacent digits of m|) from n until a value repeats; -1 if it
 *            diverges (>80 digits, conjectural). Computed by simulation on
 *            digit arrays
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9297 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

#define A329624_MAXD 400
#define A329624_LIM 80
#define A329624_MAXIT 600

static unsigned char A329624_hist[A329624_MAXIT + 1][A329624_LIM + 2];
static int A329624_hlen[A329624_MAXIT + 1];

/* x: msd-first digits, *len digits (0 = value zero). */
static void A329624_step(unsigned char *x, int *len)
{
    unsigned char c[A329624_MAXD];
    unsigned char r[A329624_MAXD];
    int cl = 0, d = *len;
    if (d == 0) return;
    for (int i = 0; i + 1 < d; i++) {
        int s = x[i] + x[i + 1];
        if (s >= 10) { c[cl++] = 1; c[cl++] = (unsigned char)(s - 10); }
        else c[cl++] = (unsigned char)s;
    }
    int cs = 0;
    while (cs < cl && c[cs] == 0) cs++;
    const unsigned char *cp = c + cs;
    int cn = cl - cs;

    const unsigned char *big, *small;
    int bl, sl, cmp;
    if (d != cn) cmp = (d > cn) ? 1 : -1;
    else cmp = memcmp(x, cp, (size_t)d);
    if (cmp == 0) { *len = 0; return; }
    if (cmp > 0) { big = x; bl = d; small = cp; sl = cn; }
    else { big = cp; bl = cn; small = x; sl = d; }

    int borrow = 0;
    for (int k = 0; k < bl; k++) {
        int bi = big[bl - 1 - k];
        int si = (k < sl) ? small[sl - 1 - k] : 0;
        int v = bi - si - borrow;
        if (v < 0) { v += 10; borrow = 1; } else borrow = 0;
        r[bl - 1 - k] = (unsigned char)v;
    }
    int rs = 0;
    while (rs < bl && r[rs] == 0) rs++;
    if (rs == bl) { *len = 0; return; }
    *len = bl - rs;
    memmove(x, r + rs, (size_t)(*len));
}

seq_t a(idx_t n)
{
    unsigned char x[A329624_MAXD];
    unsigned char t[24];
    int tl = 0, len;
    uint64_t m = (uint64_t)n;
    while (m > 0) { t[tl++] = (unsigned char)(m % 10); m /= 10; }
    len = tl;
    for (int i = 0; i < len; i++) x[i] = t[len - 1 - i];

    memcpy(A329624_hist[0], x, (size_t)len);
    A329624_hlen[0] = len;

    for (int k = 1; k <= A329624_MAXIT; k++) {
        A329624_step(x, &len);
        if (len > A329624_LIM) return (seq_t)(-1);
        for (int j = 0; j < k; j++) {
            if (A329624_hlen[j] == len &&
                (len == 0 || memcmp(A329624_hist[j], x, (size_t)len) == 0))
                return (seq_t)k;
        }
        memcpy(A329624_hist[k], x, (size_t)len);
        A329624_hlen[k] = len;
    }
    return (seq_t)(-1);
}
