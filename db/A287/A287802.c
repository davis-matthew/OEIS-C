/*
 * A287802  Positions of 0 in A287801; complement of A287803.
 * https://oeis.org/A287802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th position m where A287801(m) = 0, where A287801 is
 *            the {0->100, 1->001} transform of the Fibonacci word (computed
 *            by scanning)
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9529 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A287802_fw = NULL;
static size_t A287802_len = 0;

static void A287802_ensure(size_t need)
{
    if (A287802_len > need) return;
    unsigned char *cur = (unsigned char *)malloc(1);
    size_t len = 1;
    cur[0] = 0;
    while (len <= need) {
        unsigned char *nw = (unsigned char *)malloc(len * 2 + 1);
        size_t nl = 0;
        for (size_t i = 0; i < len; ++i) {
            if (cur[i] == 0) { nw[nl++] = 0; nw[nl++] = 1; }
            else             { nw[nl++] = 0; }
        }
        free(cur);
        cur = nw;
        len = nl;
    }
    free(A287802_fw);
    A287802_fw = cur;
    A287802_len = len;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A287802_ensure((size_t)(n / 2 + 16));
    idx_t count = 0;
    idx_t m = 0;
    for (;;) {
        ++m;
        idx_t k = (m - 1) / 3;
        int r = (int)((m - 1) % 3);
        unsigned char bit = A287802_fw[(size_t)k];
        int one = (bit == 0) ? (r == 0) : (r == 2);
        if (!one) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
