/*
 * A248393  Number of 1's in n-th "number" in A248392.
 * https://oeis.org/A248393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 1's in the n-th term of A248392 (mod-2
 *            look-and-say, term 1 = "1"); computed by direct simulation
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    size_t cap = 64, len = 1;
    uint8_t *cur = (uint8_t *)malloc(cap);
    uint8_t *nxt = (uint8_t *)malloc(cap);
    if (!cur || !nxt) { free(cur); free(nxt); return (seq_t)0; }
    cur[0] = 1;
    for (idx_t t = 1; t < n; t++) {
        if (len * 2 > cap) {
            while (len * 2 > cap) cap *= 2;
            uint8_t *c2 = (uint8_t *)realloc(cur, cap);
            if (!c2) { free(cur); free(nxt); return (seq_t)0; }
            cur = c2;
            uint8_t *n2 = (uint8_t *)realloc(nxt, cap);
            if (!n2) { free(cur); free(nxt); return (seq_t)0; }
            nxt = n2;
        }
        size_t m = 0;
        for (size_t i = 0; i < len; ) {
            size_t j = i;
            while (j < len && cur[j] == cur[i]) j++;
            nxt[m++] = (uint8_t)((j - i) & 1);
            nxt[m++] = (uint8_t)(cur[i] & 1);
            i = j;
        }
        uint8_t *tmp = cur; cur = nxt; nxt = tmp;
        len = m;
    }
    unsigned long long cnt = 0;
    for (size_t i = 0; i < len; i++) cnt += cur[i];
    free(cur);
    free(nxt);
    return (seq_t)cnt;
}
