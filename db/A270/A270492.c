/*
 * A270492  a(n) = gcd(r) where r ranges over the orders of all subgroups
 *          whose direct product gives the multiplicative group modulo n.
 * https://oeis.org/A270492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd of φ(p^e) for each odd prime factor of n and * the
 *            component orders 2 and 2^{e-2} for the power of two factor 2^e
 *            (e≥3). * The function factors n and computes the gcd accordingly
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    if (n <= 2)
        return 0;          /* U(1) and U(2) are trivial, so a=0 */

    seq_t res = 0;          /* start as 0; gcd(0, x) = x */
    idx_t temp = n;

    /* Factor n by trial division (fast enough for n up to ~10^6) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* Count exponent e of prime p */
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            if (p == 2) {
                if (exp == 1) {
                    /* U(2) contributes order 1 – skip */
                    continue;
                } else if (exp == 2) {
                    /* U(4) is cyclic of order 2 */
                    res = gcd(res, (seq_t)2);
                } else {
                    /* 2^e with e ≥ 3: U(2^e) ≅ C2 × C_{2^{e-2}} */
                    res = gcd(res, (seq_t)2);                /* component of order 2 */
                    seq_t ord = pow_int((seq_t)2, exp - 2);   /* 2^{e-2} */
                    res = gcd(res, ord);
                }
            } else { /* odd prime */
                /* φ(p^e) = (p-1) * p^{e-1} */
                seq_t phi = (seq_t)(p - 1);
                for (idx_t i = 1; i < exp; ++i)
                    phi *= (seq_t)p;
                res = gcd(res, phi);
            }
        }
    }

    /* Handle a remaining prime factor > sqrt(n) */
    if (temp > 1) {
        if (temp != 2) {            /* odd prime factor */
            seq_t phi = (seq_t)(temp - 1);   /* e = 1 */
            res = gcd(res, phi);
        }
        /* If the remaining factor is 2 (i.e., n was a power of two),
         * the component order 1 is skipped as above. */
    }

    return res;
}
