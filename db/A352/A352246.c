/*
 * A352246  Indices k where A352245(k) = 0.
 * https://oeis.org/A352246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index k with A352245(k) = 0; found by simulating
 *            A352245 (a(0)=1, a(k) = 1-based first position of binary k in
 *            the concatenated binary terms, else 0)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char *A352246_bs = NULL;
static size_t A352246_len = 0, A352246_cap = 0;
static unsigned long long *A352246_zeros = NULL;
static size_t A352246_zc = 0, A352246_zcap = 0;
static unsigned long long A352246_next = 1;

static void A352246_push_char(char c)
{
    if (A352246_len + 1 > A352246_cap) {
        size_t nc = A352246_cap ? A352246_cap * 2 : 1024;
        A352246_bs = (char *)realloc(A352246_bs, nc);
        A352246_cap = nc;
    }
    A352246_bs[A352246_len++] = c;
}

static void A352246_push_val(unsigned long long v)
{
    char buf[70];
    size_t l = 0;
    if (v == 0) { A352246_push_char('0'); return; }
    while (v) { buf[l++] = (v & 1) ? '1' : '0'; v >>= 1; }
    while (l > 0) A352246_push_char(buf[--l]);
}

seq_t a(idx_t n)
{
    if (A352246_bs == NULL) {
        A352246_push_char('1');
        A352246_zcap = 64;
        A352246_zeros = (unsigned long long *)malloc(A352246_zcap * sizeof(unsigned long long));
    }
    while (A352246_zc <= (size_t)n) {
        unsigned long long k = A352246_next++;
        char pat[70];
        size_t pl = 0, i, j;
        unsigned long long t = k;
        long long pos = -1;
        while (t) { pat[pl++] = (t & 1) ? '1' : '0'; t >>= 1; }
        for (i = 0; i < pl / 2; i++) {
            char c = pat[i]; pat[i] = pat[pl - 1 - i]; pat[pl - 1 - i] = c;
        }
        for (i = 0; i + pl <= A352246_len; i++) {
            for (j = 0; j < pl; j++)
                if (A352246_bs[i + j] != pat[j]) break;
            if (j == pl) { pos = (long long)i; break; }
        }
        if (pos < 0) {
            if (A352246_zc == A352246_zcap) {
                A352246_zcap *= 2;
                A352246_zeros = (unsigned long long *)realloc(A352246_zeros,
                                    A352246_zcap * sizeof(unsigned long long));
            }
            A352246_zeros[A352246_zc++] = k;
            A352246_push_val(0);
        } else {
            A352246_push_val((unsigned long long)pos + 1);
        }
    }
    return (seq_t)A352246_zeros[(size_t)n];
}
