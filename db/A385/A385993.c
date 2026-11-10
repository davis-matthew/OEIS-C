/*
 * A385993  a(n) is the number of distinct values appearing an odd number of
 *          times among A002487(0), ..., A002487(n).
 * https://oeis.org/A385993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values occurring an odd number of
 *            times among A002487(0..n); A002487(0)=0, A002487(1)=1,
 *            A002487(2k)=A002487(k), A002487(2k+1)=A002487(k)+A002487(k+1)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 97 terms through n = 8013 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    uint64_t *s = (uint64_t *)malloc(((size_t)n + 2) * sizeof(uint64_t));
    unsigned char *par = (unsigned char *)calloc((size_t)n + 2, 1);
    uint64_t i, cnt = 0;
    s[0] = 0;
    s[1] = 1;
    for (i = 2; i <= n; i++) {
        if (i % 2 == 0) s[i] = s[i / 2];
        else s[i] = s[i / 2] + s[i / 2 + 1];
    }
    for (i = 0; i <= n; i++) {
        uint64_t v = s[i];
        if (par[v]) { par[v] = 0; cnt--; }
        else { par[v] = 1; cnt++; }
    }
    free(s);
    free(par);
    return (seq_t)cnt;
}
