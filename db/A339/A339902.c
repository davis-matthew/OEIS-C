/*
 * A339902  Number of prime divisors of A339821(n), counted with multiplicity.
 * https://oeis.org/A339902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   if 4n = 2^e1 + ... + 2^ek then a(n) = Sum_i bigomega(prime(e_i)
 *            - 1), i.e. A023508(e1)+...+A023508(ek)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 113 terms through n = 9948 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A339902_bigomega_small(uint32_t m)
{
    unsigned c = 0;
    uint32_t d;
    for (d = 2; d * d <= m; d++) {
        while (m % d == 0) { m /= d; c++; }
    }
    if (m > 1) c++;
    return c;
}

seq_t a(idx_t n)
{
    /* bit i of n corresponds to bit e = i+2 of 4n; need prime(e) for e up to 66 */
    static uint32_t omg[70];
    static int init = 0;
    if (!init) {
        uint32_t pr[70];
        int cnt = 0;
        uint32_t c = 2;
        while (cnt < 67) {
            int ok = 1;
            int j;
            for (j = 0; j < cnt; j++) {
                if (pr[j] * pr[j] > c) break;
                if (c % pr[j] == 0) { ok = 0; break; }
            }
            if (ok) pr[cnt++] = c;
            c++;
        }
        for (cnt = 0; cnt < 67; cnt++)
            omg[cnt + 1] = A339902_bigomega_small(pr[cnt] - 1); /* omg[e] = bigomega(prime(e)-1) */
        init = 1;
    }
    seq_t res = 0;
    uint64_t m = (uint64_t)n;
    int e = 2;
    while (m) {
        if (m & 1ULL) res += (seq_t)omg[e];
        m >>= 1;
        e++;
    }
    return res;
}
