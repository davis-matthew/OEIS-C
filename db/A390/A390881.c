/*
 * A390881  a(n) = gcd(A018804(n), A000203(n)).
 * https://oeis.org/A390881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A018804(n), A000203(n)); A018804 = Prod_{p^e||n}
 *            p^(e-1)*((e+1)p - e), A000203 = Prod_{p^e||n} (p^(e+1)-1)/(p-1)
 * Range:     u8 n <= 55, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A390881_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t pillai = 1;
    seq_t sig = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t pw = 1;      /* p^(e-1) after loop */
            seq_t term = 1;    /* 1 + p + ... + p^e */
            seq_t prev = 1;
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                prev = pw;
                pw *= (seq_t)p;
                term += pw;
                e++;
            }
            /* prev = p^(e-1) */
            pillai *= prev * (((seq_t)e + 1) * (seq_t)p - (seq_t)e);
            sig *= term;
        }
    }
    if (m > 1) {
        pillai *= (2 * (seq_t)m - 1);
        sig *= ((seq_t)m + 1);
    }
    return A390881_gcd(pillai, sig);
}
