/*
 * A286617  Restricted growth sequence of A278217 (prime-signature of
 *          A075159(1+n)).
 * https://oeis.org/A286617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = restricted growth sequence of A278217(n) =
 *            A046523(A075159(n+1)); computed by comparing sorted exponent
 *            vectors (prime signatures) of A075159(k+1), k = 0..n, and
 *            numbering them in order of first appearance from 1
 * Range:     u8 n <= 33215, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 125 terms through n = 9673 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A286617_REC 66

static void A286617_sig(uint64_t m, unsigned char *s)
{
    unsigned char runs[70];
    int rc = 0, cnt = 0, i, j;
    s[0] = 0;
    if (m == 0) return;
    {
        int cur = (int)(m & 1);
        int len = 0;
        while (m) {
            int bit = (int)(m & 1);
            if (bit == cur) len++;
            else { runs[rc++] = (unsigned char)len; cur = bit; len = 1; }
            m >>= 1;
        }
        runs[rc++] = (unsigned char)len;
    }
    for (i = 0; i < rc; i++) {
        int e = runs[i];
        if (i != rc - 1 && e > 0) e--;
        if (e > 0) s[1 + cnt++] = (unsigned char)e;
    }
    for (i = 1; i < cnt; i++) {
        unsigned char v = s[1 + i];
        j = i - 1;
        while (j >= 0 && s[1 + j] > v) { s[2 + j] = s[1 + j]; j--; }
        s[1 + j + 1] = v;
    }
    s[0] = (unsigned char)cnt;
}

seq_t a(idx_t n)
{
    size_t cap = 64, used = 0;
    unsigned char *tab = (unsigned char *)malloc(cap * A286617_REC);
    unsigned char s[A286617_REC];
    uint64_t j;
    size_t id = 0;
    for (j = 0; j <= (uint64_t)n; j++) {
        size_t k;
        int found = 0;
        memset(s, 0, sizeof(s));
        A286617_sig(j, s);
        for (k = 0; k < used; k++) {
            unsigned char *r = tab + k * A286617_REC;
            if (r[0] == s[0] && memcmp(r + 1, s + 1, s[0]) == 0) { found = 1; id = k; break; }
        }
        if (!found) {
            if (used == cap) {
                cap *= 2;
                tab = (unsigned char *)realloc(tab, cap * A286617_REC);
            }
            memcpy(tab + used * A286617_REC, s, A286617_REC);
            id = used++;
        }
    }
    free(tab);
    return (seq_t)(id + 1);
}
