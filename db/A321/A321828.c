/*
 * A321828  a(n) = Sum_{d|n, d==1 mod 4} d^12 - Sum_{d|n, d==3 mod 4} d^12.
 * https://oeis.org/A321828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, d≡1 (mod 4)} d^12 – Sum_{d|n, d≡3 (mod 4)} d^12
 *            (OEIS A321828) Computation is multiplicative: a(n) = ∏_{p^e||n}
 *            g(p,e) where g(2,e) = 1 g(p,e) = ∑_{k=0..e} p^{12k} if p≡1 (mod
 *            4) g(p,e) = ∑_{k=0..e} (-1)^k p^{12k} if p≡3 (mod 4)
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 6, i64 n <= 38, i128 n <= 1534,
 *            big any n
 * Helpers:   pow_seq_long
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_long.h"

seq_t a(idx_t n) {
    if (n <= 1)          /* n==1 gives 1 as required */
        return 1;
    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* handle factor 2 separately (contribution is always 1) */
    while ((m & 1ULL) == 0) { /* divide out all 2's */
        m >>= 1;
    }

    /* trial division for the remaining odd part */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            seq_t base = pow_seq_long((seq_t)p, 12);   /* p^12   */
            seq_t contrib = 1;                         /* k = 0 term 1 */
            seq_t term = 1;                            /* current p^(12*k) */

            if (p % 4 == 1) {
                /* geometric series 1 + base + base^2 + ... + base^e */
                for (int k = 1; k <= e; ++k) {
                    term *= base;                     /* base^k */
                    contrib += term;
                }
            } else { /* p % 4 == 3 */
                /* alternating signs, start with +1 at k=0 */
                for (int k = 1; k <= e; ++k) {
                    term *= base;                     /* base^k */
                    if ((k & 1) == 0)                 /* even k => + */
                        contrib += term;
                    else                               /* odd k  => - */
                        contrib -= term;
                }
            }
            result *= contrib;
        }
    }

    /* if after trial division we still have a prime factor >1 */
    if (m > 1) {
        uint64_t p = m;              /* remaining prime */
        int e = 1;                   /* exponent is 1 */
        seq_t base = pow_seq_long((seq_t)p, 12);
        seq_t contrib = 1;
        seq_t term = 1;
        if (p % 4 == 1) {
            for (int k = 1; k <= e; ++k) {
                term *= base;
                contrib += term;
            }
        } else {
            for (int k = 1; k <= e; ++k) {
                term *= base;
                if ((k & 1) == 0)
                    contrib += term;
                else
                    contrib -= term;
            }
        }
        result *= contrib;
    }

    return result;
}
