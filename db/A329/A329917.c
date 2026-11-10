/*
 * A329917  Starting values for which iterations of A329623 diverge
 *          (conjectural).
 * https://oeis.org/A329917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number m whose trajectory under iteration of A329623
 *            (m -> |m - concat of sums of adjacent digits of m|) grows
 *            without bound. Found by search: iterate each candidate m on
 *            digit arrays. It diverges if the iterate exceeds 80 digits, and
 *            converges if it reaches a fixed point or 0, or sur...
 * Range:     u8 none, u16 n <= 13047, u32 n <= 13047, u64 n <= 13047,
 *            u128 n <= 13047, big any n
 * Validated: standard, pass, 68 terms through n = 8822 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A329917_MAXD 400
#define A329917_LIM 80

/* x: msd-first digits, *len digits. Returns 0 if result is zero. */
static int A329917_step(unsigned char *x, int *len)
{
    unsigned char c[A329917_MAXD];
    unsigned char r[A329917_MAXD];
    int cl = 0, d = *len;
    for (int i = 0; i + 1 < d; i++) {
        int s = x[i] + x[i + 1];
        if (s >= 10) { c[cl++] = 1; c[cl++] = (unsigned char)(s - 10); }
        else c[cl++] = (unsigned char)s;
    }
    /* strip leading zeros of c */
    int cs = 0;
    while (cs < cl && c[cs] == 0) cs++;
    const unsigned char *cp = c + cs;
    int cn = cl - cs;

    const unsigned char *big, *small;
    int bl, sl;
    int cmp;
    if (d != cn) cmp = (d > cn) ? 1 : -1;
    else cmp = memcmp(x, cp, (size_t)d);
    if (cmp == 0) return 0;
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
    if (rs == bl) return 0;
    *len = bl - rs;
    memcpy(x, r + rs, (size_t)(*len));
    return 1;
}

static bool A329917_diverges(uint64_t m)
{
    unsigned char x[A329917_MAXD], prev[A329917_MAXD];
    unsigned char t[24];
    int tl = 0, len;
    while (m > 0) { t[tl++] = (unsigned char)(m % 10); m /= 10; }
    len = tl;
    for (int i = 0; i < len; i++) x[i] = t[len - 1 - i];

    for (int it = 0; it < 600; it++) {
        int plen = len;
        memcpy(prev, x, (size_t)len);
        if (!A329917_step(x, &len)) return false;
        if (len > A329917_LIM) return true;
        if (len == plen && memcmp(prev, x, (size_t)len) == 0) return false;
    }
    return false;
}

static uint64_t *A329917_terms = NULL;
static size_t A329917_cnt = 0, A329917_cap = 0;
static uint64_t A329917_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (A329917_cnt < (size_t)n) {
        uint64_t m = A329917_next++;
        if (A329917_diverges(m)) {
            if (A329917_cnt == A329917_cap) {
                A329917_cap = A329917_cap ? A329917_cap * 2 : 1024;
                A329917_terms = (uint64_t *)realloc(A329917_terms,
                                                    A329917_cap * sizeof(uint64_t));
            }
            A329917_terms[A329917_cnt++] = m;
        }
    }
    return (seq_t)A329917_terms[n - 1];
}
