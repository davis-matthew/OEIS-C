/*
 * A276739  Least k such that A045876(k) is divisible by 10^n.
 * https://oeis.org/A276739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = least k with 10^n | A045876(k); A045876(k) =
 *            ((10^d-1)/9)*S*N/d depends only on the digit multiset of k (d
 *            digits, digit sum S, N distinct permutations). Search by digit
 *            count d, enumerating digit multisets and testing 2- and 5-adic
 *            valuations; the smallest k is that multiset's minimal arr...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 4 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A276739_freq[10];
static int A276739_n;
static int A276739_found;
static seq_t A276739_best;

static int A276739_vpf(int m, int p)
{
    int r = 0;
    while (m) { m /= p; r += m; }
    return r;
}

static int A276739_vp(int m, int p)
{
    int r = 0;
    while (m > 0 && m % p == 0) { m /= p; r++; }
    return r;
}

static void A276739_eval(int d)
{
    int S = 0, i;
    for (i = 0; i < 10; i++) S += i * A276739_freq[i];
    if (S == 0) return;
    int ps[2] = {2, 5};
    for (int t = 0; t < 2; t++) {
        int p = ps[t];
        int v = A276739_vp(S, p) + A276739_vpf(d, p) - A276739_vp(d, p);
        for (i = 0; i < 10; i++) v -= A276739_vpf(A276739_freq[i], p);
        /* repunit contributes no factors of 2 or 5 */
        if (v < A276739_n) return;
    }
    /* minimal arrangement */
    int f[10];
    for (i = 0; i < 10; i++) f[i] = A276739_freq[i];
    seq_t k = 0;
    for (i = 1; i < 10; i++) {
        if (f[i] > 0) { k = (seq_t)i; f[i]--; break; }
    }
    for (i = 0; i < 10; i++) {
        while (f[i] > 0) { k = k * 10 + (seq_t)i; f[i]--; }
    }
    if (!A276739_found || k < A276739_best) {
        A276739_best = k;
        A276739_found = 1;
    }
}

static void A276739_rec(int digit, int remaining, int d)
{
    if (digit == 9) {
        A276739_freq[9] = remaining;
        A276739_eval(d);
        return;
    }
    for (int c = 0; c <= remaining; c++) {
        A276739_freq[digit] = c;
        A276739_rec(digit + 1, remaining - c, d);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    A276739_n = (int)n;
    for (int d = 1; d <= 30; d++) {
        A276739_found = 0;
        A276739_best = 0;
        A276739_rec(0, d, d);
        if (A276739_found) return A276739_best;
    }
    return 0;
}
