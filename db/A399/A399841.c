/*
 * A399841  a(n) = A087935(n) mod n.
 * https://oeis.org/A399841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A087935(n) mod n, where A087935(n) = A087935(n-4) +
 *            A087935(n-5), A087935(0)=5, A087935(1)=A087935(2)=A087935(3)=0,
 *            A087935(4)=4 (computed mod n)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9663 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t w[5];
    w[0] = 5 % m;
    w[1] = 0;
    w[2] = 0;
    w[3] = 0;
    w[4] = 4 % m;
    if (n <= 4) {
        return w[(int)n];
    }
    /* w[k] holds P(i-5+k) before computing P(i) */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t v = w[1] + w[0];
        if (v >= m) v -= m;
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = v;
    }
    return w[4];
}
