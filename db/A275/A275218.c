/*
 * A275218  Numbers in 2-cycles of RATS sequences.
 * https://oeis.org/A275218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number k with A036839(A036839(k)) = k, where
 *            A036839(k) = sort digits of (k + reverse(k)); found by
 *            enumerating digit-sorted numbers by length
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 14, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#define A275218_MAXL 60

static int A275218_rats(const int *d, int len, int *o)
{
    int s[A275218_MAXL + 4];
    int c = 0, i, sl = len, k = 0, dg, j;
    int cnt[10] = {0};
    for (i = 0; i < len; i++) {
        int t = d[len - 1 - i] + d[i] + c;
        s[i] = t % 10;
        c = t / 10;
    }
    if (c) s[sl++] = c;
    for (i = 0; i < sl; i++) cnt[s[i]]++;
    for (dg = 1; dg < 10; dg++)
        for (j = 0; j < cnt[dg]; j++) o[k++] = dg;
    return k;
}

static unsigned char *A275218_store = 0;
static size_t A275218_found = 0, A275218_cap = 0;
static int A275218_nextLen = 1;
static int A275218_cur[A275218_MAXL + 4];
static int A275218_L;

static void A275218_gen(int pos, int mn)
{
    int dg;
    if (pos == A275218_L) {
        int o1[A275218_MAXL + 4], o2[A275218_MAXL + 4];
        int m = A275218_rats(A275218_cur, A275218_L, o1);
        int k = A275218_rats(o1, m, o2);
        if (k == A275218_L && memcmp(o2, A275218_cur, sizeof(int) * (size_t)k) == 0) {
            int i;
            if (A275218_found == A275218_cap) {
                A275218_cap = A275218_cap ? A275218_cap * 2 : 64;
                A275218_store = (unsigned char *)realloc(A275218_store, A275218_cap * (A275218_MAXL + 1));
            }
            A275218_store[A275218_found * (A275218_MAXL + 1)] = (unsigned char)A275218_L;
            for (i = 0; i < A275218_L; i++)
                A275218_store[A275218_found * (A275218_MAXL + 1) + 1 + i] = (unsigned char)A275218_cur[i];
            A275218_found++;
        }
        return;
    }
    for (dg = mn; dg <= 9; dg++) {
        A275218_cur[pos] = dg;
        A275218_gen(pos + 1, dg);
    }
}

seq_t a(idx_t n)
{
    seq_t v = (seq_t)0;
    int i, len;
    unsigned char *p;
    while ((idx_t)A275218_found < n && A275218_nextLen <= A275218_MAXL) {
        A275218_L = A275218_nextLen++;
        A275218_gen(0, 1);
    }
    if (n < 1 || (idx_t)A275218_found < n) return (seq_t)0;
    p = A275218_store + (size_t)(n - 1) * (A275218_MAXL + 1);
    len = p[0];
    for (i = 0; i < len; i++) v = v * (seq_t)10 + (seq_t)p[1 + i];
    return v;
}
