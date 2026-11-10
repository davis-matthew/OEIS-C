/*
 * A276387  Write A003512(n) in the base {1, 3, 4, 11, 15, 41, 56, 153, 209,
 *          ...} (see A002530).
 * https://oeis.org/A276387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A003512(n) written in the greedy base
 *            {1,3,4,11,15,41,56,...} (A002530(k+2), k>=0), digits read as a
 *            decimal number
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 209, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 48 terms through n = 951 (2026-11-08)
 * Helpers:   isqrt_seq, isqrt_u128, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"
#include "../utils/isqrt_u128.h"
#include "../utils/mem.h"

static seq_t A003512_a(idx_t n) {
    if (n < ((idx_t)1 << 62)) {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 r = 2 * N + isqrt_u128(3 * N * N);
        return (seq_t)(uint64_t)r;
    }
    seq_t m = (seq_t)n;
    return 2 * m + isqrt_seq(3 * m * m);
}

seq_t a(idx_t n) {
    seq_t m = A003512_a(n);
    const int CAP = 600;
    seq_t *w = mem_new(seq_t, CAP);
    w[0] = 1;
    w[1] = 3;
    w[2] = 4;
    w[3] = 11;
    int cnt = 4;
    while (cnt < CAP && w[cnt - 1] <= m) {
        w[cnt] = 4 * w[cnt - 2] - w[cnt - 4];
        cnt++;
    }
    int top = cnt - 1;
    while (top > 0 && w[top] > m) top--;
    seq_t res = 0;
    for (int k = top; k >= 0; k--) {
        seq_t d = m / w[k];
        m = m % w[k];
        res = res * 10 + d;
    }
    mem_free(w);
    return res;
}
