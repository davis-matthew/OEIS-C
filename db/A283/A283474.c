/*
 * A283474  a(0) = 0, a(1) = 1, for n > 1, a(n) = a(n-1) + a(n-A002487(n)).
 * https://oeis.org/A283474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; for n > 1, a(n) = a(n-1) + a(n - A002487(n))
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 83, u64 n <= 223,
 *            u128 n <= 607, big any n
 * Validated: standard, pass, 66 terms through n = 999 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static uint64_t A002487_stern(idx_t n) {
    uint64_t x = 1, y = 0;
    while (n > 0) {
        if (n & 1) y += x; else x += y;
        n >>= 1;
    }
    return y;
}

static seq_t *A283474_tab = NULL;
static idx_t A283474_cnt = 0;

seq_t a(idx_t n) {
    if (n >= A283474_cnt) {
        idx_t newcnt = n + 1;
        if (newcnt < 2 * A283474_cnt) newcnt = 2 * A283474_cnt;
        if (newcnt < 16) newcnt = 16;
        A283474_tab = mem_resize(seq_t, A283474_tab, (size_t)newcnt);
        idx_t i = A283474_cnt;
        if (i == 0) {
            A283474_tab[0] = 0;
            A283474_tab[1] = 1;
            i = 2;
        }
        for (; i < newcnt; i++) {
            idx_t j = i - (idx_t)A002487_stern(i);
            A283474_tab[i] = A283474_tab[i - 1] + A283474_tab[j];
        }
        A283474_cnt = newcnt;
    }
    return A283474_tab[n];
}
