/*
 * A352566  a(n) = number of modules with n elements over the ring of integers
 *          in the imaginary quadratic field of discriminant -15.
 * https://oeis.org/A352566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e||n} f_p(e), where f_p(e) = P(e) if p | 15
 *            (ramified) B(e) if p = 2 or (−15/p) = 1 (split) 0 if p ∤ 15,
 *            (−15/p) = −1 and e is odd P(e/2) if p ∤ 15, (−15/p) = −1 and e
 *            even P(k) = partition number of k, B(k) = number of bipartitions
 *            of k (sum_{i=0}^k P(i)·P(k-i))
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/powmod64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */

    /* Pre‑compute partitions and bipartitions up to exponent 13 */
    static bool init = false;
    static seq_t part[14];
    static seq_t bip[14];
    if (!init) {
        init = true;
        const idx_t MAX = 14;                 /* need exponents up to 13 */
        for (idx_t i = 0; i < MAX; ++i) part[i] = 0;
        part[0] = 1;
        for (idx_t k = 1; k < MAX; ++k)
            for (idx_t s = k; s < MAX; ++s)
                part[s] += part[s - k];

        for (idx_t m = 0; m < MAX; ++m) {
            seq_t sum = 0;
            for (idx_t i = 0; i <= m; ++i)
                sum += part[i] * part[m - i];
            bip[m] = sum;
        }
    }

    seq_t result = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t val;
            if (p == 3 || p == 5) {                 /* ramified */
                val = part[e];
            } else if (p == 2) {                   /* split: (−15/2)=1 */
                val = bip[e];
            } else {                               /* p odd, p ∤ 15 */
                idx_t t = 15 % p;                  /* 15 mod p */
                idx_t x = (p - t) % p;             /* x = (−15) mod p */
                idx_t ls = powmod64(x, (p - 1) / 2, p);
                bool split = (ls == 1);
                if (split) {
                    val = bip[e];
                } else {                           /* inert */
                    if (e & 1) return 0;
                    val = part[e / 2];
                }
            }
            result *= val;
            if (result == 0) return 0;
        }
    }
    if (tmp > 1) {                                 /* remaining prime factor */
        idx_t p = tmp;
        idx_t e = 1;
        seq_t val;
        if (p == 3 || p == 5) {
            val = part[e];
        } else if (p == 2) {
            val = bip[e];
        } else {
            idx_t t = 15 % p;
            idx_t x = (p - t) % p;
            idx_t ls = powmod64(x, (p - 1) / 2, p);
            bool split = (ls == 1);
            if (split) {
                val = bip[e];
            } else {
                if (e & 1) return 0;
                val = part[e / 2];
            }
        }
        result *= val;
    }
    return result;
}
