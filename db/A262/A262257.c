/*
 * A262257  Minimal number of editing steps (delete, insert or substitute) to
 *          transform n in decimal representation into the largest palindrome
 *          <= n.
 * https://oeis.org/A262257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Levenshtein distance between decimal representation of n
 *            and the largest palindrome ≤ n (OEIS A262257)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* return number of decimal digits of an idx_t value (0 → 1 digit) */
static inline int num_digits(idx_t x)
{
    if (x == 0) return 1;
    int d = 0;
    while (x != 0) {
        x /= 10;
        d++;
    }
    return d;
}

/* Levenshtein distance between the decimal strings of two 64‑bit integers */
static inline seq_t levenshtein(idx_t n, idx_t m)
{
    int nd = num_digits(n);
    int md = num_digits(m);
    uint8_t ndigits[20];
    uint8_t mdigits[20];
    idx_t tmp = n;
    for (int i = nd - 1; i >= 0; --i) {
        ndigits[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    tmp = m;
    for (int i = md - 1; i >= 0; --i) {
        mdigits[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    int dist[21][21];
    for (int i = 0; i <= nd; ++i) dist[i][0] = i;
    for (int j = 0; j <= md; ++j) dist[0][j] = j;
    for (int i = 1; i <= nd; ++i) {
        for (int j = 1; j <= md; ++j) {
            int sub = dist[i - 1][j - 1] + (ndigits[i - 1] == mdigits[j - 1] ? 0 : 1);
            int del = dist[i - 1][j] + 1;
            int ins = dist[i][j - 1] + 1;
            int mval = sub;
            if (del < mval) mval = del;
            if (ins < mval) mval = ins;
            dist[i][j] = mval;
        }
    }
    return (seq_t)dist[nd][md];
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* extract digits of n */
    int len = num_digits(n);
    uint8_t digits[20];
    idx_t t = n;
    for (int i = len - 1; i >= 0; --i) {
        digits[i] = (uint8_t)(t % 10);
        t /= 10;
    }

    /* build initial palindrome candidate by mirroring */
    uint8_t pal[20];
    for (int i = 0; i < len; ++i) pal[i] = digits[i];
    for (int i = 0; i < len / 2; ++i) pal[len - 1 - i] = pal[i];

    /* compare candidate with n */
    int cmp = 0;
    for (int i = 0; i < len; ++i) {
        if (pal[i] < digits[i]) { cmp = -1; break; }
        if (pal[i] > digits[i]) { cmp = 1;  break; }
    }

    idx_t palindrome = 0;

    if (cmp <= 0) {
        /* candidate is ≤ n */
        for (int i = 0; i < len; ++i)
            palindrome = palindrome * 10 + pal[i];
    } else {
        /* decrement the middle part to get the greatest smaller palindrome */
        int pos = (len - 1) / 2;
        while (pos >= 0 && pal[pos] == 0) {
            pal[pos] = 9;
            pos--;
        }
        if (pos >= 0) pal[pos]--;
        if (pal[0] == 0) {
            /* all leading zeros → 99…9 of length len‑1 */
            for (int i = 0; i < len - 1; ++i)
                palindrome = palindrome * 10 + 9;
        } else {
            /* mirror again */
            for (int i = 0; i < len / 2; ++i)
                pal[len - 1 - i] = pal[i];
            for (int i = 0; i < len; ++i)
                palindrome = palindrome * 10 + pal[i];
        }
    }

    return levenshtein(n, palindrome);
}
