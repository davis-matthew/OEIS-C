/*
 * A250983  First differences of A094589.
 * https://oeis.org/A250983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest term of A094589 that is <= n; A094589: b(1)=1,
 *            b(m+1) = b(m) + (largest b(k) <= m). Equivalently a(n) = b(n+1)
 *            - b(n)
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9744 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t cap = 64, cnt = 0, kp = 0;
    seq_t *t = mem_new(seq_t, cap);
    seq_t N = (seq_t)n;
    seq_t cur = 1, best = 1;
    size_t m = 1;
    for (;;) {
        if (cur > N) break;
        best = cur;
        if (cnt == cap) {
            cap <<= 1;
            t = mem_resize(seq_t, t, cap);
        }
        t[cnt++] = cur;                  /* t[i] = b(i+1) */
        /* largest b(k) <= m */
        while (kp + 1 < cnt && t[kp + 1] <= (seq_t)m) kp++;
        cur = cur + t[kp];
        m++;
    }
    mem_free(t);
    return best;
}
