/*
 * A252117  Irregular triangle read by row: T(n,k), n>=1, k>=1, in which
 *          column k lists the numbers of A000716 multiplied by A000330(k),
 *          and the first element of column k is in row A000217(k).
 * https://oeis.org/A252117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = A000716(n - k(k+1)/2) * k(k+1)(2k+1)/6, 1 <= k <=
 *            A003056(n); a(m) is the m-th entry reading rows. A000716:
 *            a(0)=1, a(n)=Sum_{j>=1} (-1)^(j+1)*(2j+1)*a(n-j(j+1)/2)
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A252117_tab = NULL;
static idx_t A252117_len = 0;

static void A252117_fill(idx_t upto)
{
    if (upto < A252117_len) return;
    idx_t newlen = A252117_len * 2;
    if (newlen < upto + 1) newlen = upto + 1;
    if (newlen < 64) newlen = 64;
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    for (idx_t i = 0; i < A252117_len; i++) p[i] = A252117_tab[i];
    if (A252117_tab) mem_free(A252117_tab);
    A252117_tab = p;
    for (idx_t i = A252117_len; i < newlen; i++) {
        if (i == 0) { p[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; k * (k + 1) / 2 <= i; k++) {
            idx_t t = k * (k + 1) / 2;
            seq_t term = (seq_t)(2 * k + 1) * p[i - t];
            if (k & 1) pos += term;
            else neg += term;
        }
        p[i] = pos - neg;
    }
    A252117_len = newlen;
}

seq_t a(idx_t n)
{
    idx_t pos = n, r = 1, L = 1;
    while (pos > L) {
        pos -= L;
        r++;
        if ((L + 1) * (L + 2) / 2 <= r) L++;
    }
    idx_t k = pos;
    idx_t idx = r - k * (k + 1) / 2;
    A252117_fill(idx);
    seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;
    seq_t sq = tri * (2 * (seq_t)k + 1) / 3;
    return A252117_tab[idx] * sq;
}
