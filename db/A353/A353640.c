/*
 * A353640  Čiurlionis sequence A342002, reduced modulo 4.
 * https://oeis.org/A353640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A342002(n) mod 4, where A342002(n) = rad(m) *
 *            Sum_{p^e||m} e/p with m = A276086(n); computed directly mod 4
 *            from the primorial-base digits of n
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9826 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    unsigned d[20];
    unsigned cnt = 0;
    idx_t t = n;
    unsigned i, j;
    uint64_t res = 0;

    for (i = 0; i < 20; i++) {
        d[i] = (unsigned)(t % pr[i]);
        t /= pr[i];
        if (d[i] != 0) cnt = i + 1;
        if (t == 0) break;
    }
    for (i = 0; i < cnt; i++) {
        uint64_t prod;
        if (d[i] == 0) continue;
        prod = d[i];
        for (j = 0; j < cnt; j++) {
            if (j != i && d[j] != 0) prod = (prod * (uint64_t)pr[j]) & 3u;
        }
        res = (res + prod) & 3u;
    }
    return (seq_t)res;
}
