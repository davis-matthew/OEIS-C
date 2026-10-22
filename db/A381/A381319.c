/*
 * A381319  Order of linear recurrence with constant coefficients of solutions
 *          of k satisfying k^(n-1) == 1 (mod n^2) for a given n.
 * https://oeis.org/A381319
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (# of solutions k to k^(n-1) ≡ 1 (mod n^2),
 *            gcd(k,n)=1) The number of solutions equals the product over the
 *            prime powers p^e | n of gcd(n-1, |U(p^(2e))|). For odd p, |U| is
 *            cyclic with order |U| = p^(2e-1)(p-1). For p=2, U(2^k) ≅ C2 ×
 *            C2^(k-2) (k≥3) or cyclic for k=2. Thus the count fo...
 * Range:     u8 n <= 18, u16 n <= 1606, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n<2, harmless */

    /* d = n-1, counted in seq_t to avoid overflow in gcd */
    seq_t d = (seq_t)n - 1;
    seq_t count = 1;

    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) { m /= p; ++e; }

            if (p == 2) {
                idx_t exp2 = 2 * e;          /* exponent of 2 in n^2 */
                /* For k=2 (exp2==2) the group is cyclic of order 2 */
                count *= gcd(d, (seq_t)2);
                if (exp2 > 2) {
                    /* U(2^k) ≅ C2 × C2^(k-2) for k>=3 */
                    seq_t pow2 = ((seq_t)1) << (exp2 - 2);  /* 2^(k-2) */
                    count *= gcd(d, pow2);
                }
            } else {
                /* odd prime power: U(p^(2e)) is cyclic of order φ=p^(2e-1)(p-1) */
                seq_t pow = 1;
                for (idx_t i = 0; i < 2 * e - 1; ++i) pow *= (seq_t)p;
                seq_t phi = pow * (seq_t)(p - 1);
                count *= gcd(d, phi);
            }
        }
    }

    /* any remaining prime factor (m > 1) */
    if (m > 1) {
        idx_t p = m;
        if (p == 2) {
            count *= gcd(d, (seq_t)2);  /* exp2==2 case */
        } else {
            seq_t phi = (seq_t)p * (seq_t)(p - 1);
            count *= gcd(d, phi);
        }
    }

    return count + 1;   /* a(n) = 1 + count of solutions */
}
