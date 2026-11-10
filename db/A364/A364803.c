/*
 * A364803  Smallest number that reaches a fixed point after n iterations of
 *          the map x -> A022290(x).
 * https://oeis.org/A364803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(0)=0, a(1)=4, a(n) = g(a(n-1)) for n>=2, where g(m) is the
 *            smallest k with A022290(k) = m (greedy choice of the lowest top
 *            bit; Fibonacci weights F(b+2) for bit b)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* smallest k such that A022290(k) = m, for m >= 0 */
static seq_t A364803_minpre(seq_t m)
{
    const int N = 300;
    seq_t *fb = mem_new(seq_t, N);
    int cnt = 2;
    fb[0] = 0;
    fb[1] = 1;
    while (cnt < N) {
        seq_t nx = fb[cnt - 1] + fb[cnt - 2];
        if (nx < fb[cnt - 1]) break; /* overflow in narrow types */
        fb[cnt] = nx;
        ++cnt;
    }
    seq_t res = 0;
    while (m > 0) {
        int t = 0;
        /* smallest t with F(t+4) - 2 >= m */
        while (t + 4 < cnt && fb[t + 4] - 2 < m) ++t;
        if (t + 4 >= cnt) break;
        res += ((seq_t)1) << t;
        m -= fb[t + 2];
    }
    mem_free(fb);
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t x = 4;
    for (idx_t i = 2; i <= n; ++i) {
        x = A364803_minpre(x);
    }
    return x;
}
