/*
 * A348494  a(n) = A348492(n) / A003557(n), where A348492 is the GCD of the
 *          arithmetic derivative (A003415) and Pillai's arithmetical function
 *          (A018804).
 * https://oeis.org/A348494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003415(n), A018804(n)) / A003557(n); A018804
 *            multiplicative with A018804(p^e) = p^(e-1)*((e+1)*p - e)
 * Range:     u8 n <= 62, u16 n <= 7174, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9846 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    seq_t sn = (seq_t)n;
    seq_t der = 0, pil = 1, flat = 1;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            seq_t sp = (seq_t)p;
            der += (sn / sp) * (seq_t)e;
            seq_t pw = 1;
            for (int i = 1; i < e; ++i) pw *= sp;
            flat *= pw;
            pil *= pw * (((seq_t)e + 1) * sp - (seq_t)e);
        }
    }
    if (t > 1) {
        seq_t sp = (seq_t)t;
        der += sn / sp;
        pil *= (2 * sp - 1);
    }
    return gcd(der, pil) / flat;
}
