/*
 * A385991  a(n) is the number of distinct values among A002487(0), ...,
 *          A002487(n).
 * https://oeis.org/A385991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values among A002487(0..n), where
 *            A002487(0)=0, A002487(1)=1, A002487(2k)=A002487(k),
 *            A002487(2k+1)=A002487(k)+A002487(k+1); computed by sieve with
 *            running distinct count
 * Range:     u8 n <= 4954, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9303 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A385991_cnt = 0;
static size_t A385991_have = 0;

static void A385991_build(size_t cap) {
    uint64_t *s = (uint64_t *)malloc((cap + 2) * sizeof(uint64_t));
    unsigned char *seen = (unsigned char *)calloc(cap + 3, 1);
    uint32_t *c = (uint32_t *)malloc((cap + 1) * sizeof(uint32_t));
    size_t k;
    uint32_t count = 0;
    for (k = 0; k <= cap; k++) {
        uint64_t v;
        if (k == 0) v = 0;
        else if (k == 1) v = 1;
        else if (k & 1) v = s[k / 2] + s[k / 2 + 1];
        else v = s[k / 2];
        s[k] = v;
        if (v <= cap + 1 && !seen[v]) {
            seen[v] = 1;
            count++;
        }
        c[k] = count;
    }
    free(s);
    free(seen);
    free(A385991_cnt);
    A385991_cnt = c;
    A385991_have = cap + 1;
}

seq_t a(idx_t n) {
    size_t cap;
    if (A385991_cnt == 0 || (size_t)n >= A385991_have) {
        cap = A385991_have ? A385991_have * 2 : 4096;
        while (cap <= (size_t)n) cap *= 2;
        A385991_build(cap);
    }
    return (seq_t)A385991_cnt[(size_t)n];
}
