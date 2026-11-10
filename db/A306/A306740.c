/*
 * A306740  Numbers k such that A306607(k) = 0.
 * https://oeis.org/A306740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n-th k >= 0 (in increasing order) with A306607(k) = 0,
 *            where A306607(k) = Sum_{j=0..m}
 *            (-1)^(m-j)*binomial(m,j)*bit_j(k), m = floor(log2 k). Found by
 *            an incremental search, cached across calls
 * Range:     u8 n <= 19, u16 n <= 279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9677 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int64_t A306740_tab[64][8][256];
static unsigned char A306740_built[64];
static uint64_t A306740_pas[64][64];
static int A306740_pas_init = 0;

static void A306740_build(int m) {
    if (!A306740_pas_init) {
        for (int i = 0; i < 64; i++) {
            A306740_pas[i][0] = 1;
            for (int j = 1; j <= i; j++)
                A306740_pas[i][j] = A306740_pas[i-1][j-1] + (j <= i-1 ? A306740_pas[i-1][j] : 0);
        }
        A306740_pas_init = 1;
    }
    int64_t s[64];
    for (int j = 0; j < 64; j++) {
        if (j > m) { s[j] = 0; continue; }
        int64_t v = (int64_t)A306740_pas[m][j];
        s[j] = ((m - j) & 1) ? -v : v;
    }
    for (int c = 0; c < 8; c++)
        for (int v = 0; v < 256; v++) {
            int64_t t = 0;
            for (int b = 0; b < 8; b++)
                if ((v >> b) & 1) t += s[8 * c + b];
            A306740_tab[m][c][v] = t;
        }
    A306740_built[m] = 1;
}

static idx_t *A306740_terms = 0;
static idx_t A306740_cnt = 0, A306740_cap = 0;
static idx_t A306740_next = 0;

seq_t a(idx_t n) {
    while (A306740_cnt < n) {
        idx_t k = A306740_next++;
        int zero = 0;
        if (k == 0) zero = 1;
        else {
            int m = 63;
            while (!((k >> m) & 1)) m--;
            if (m > 62) continue;
            if (!A306740_built[m]) A306740_build(m);
            int64_t sum = 0;
            idx_t t = k;
            for (int c = 0; t; c++, t >>= 8)
                sum += A306740_tab[m][c][t & 255];
            zero = (sum == 0);
        }
        if (zero) {
            if (A306740_cnt == A306740_cap) {
                A306740_cap = A306740_cap ? A306740_cap * 2 : 1024;
                A306740_terms = (idx_t *)realloc(A306740_terms, A306740_cap * sizeof(idx_t));
            }
            A306740_terms[A306740_cnt++] = k;
        }
    }
    return (seq_t)A306740_terms[n - 1];
}
