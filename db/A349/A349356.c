/*
 * A349356  Dirichlet convolution of A003959 with A097945 (Dirichlet inverse
 *          of A003958), where A003958 and A003959 are fully multiplicative
 *          with a(p) = p-1 and p+1 respectively.
 * https://oeis.org/A349356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} 2*(p+1)^(e-1) (multiplicative, a(1)=1)
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 7348, u64 n <= 7348,
 *            u128 n <= 7348, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard for completeness */
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            seq_t factor = (seq_t)2;          /* base factor for this prime */

            if (e > 1) {                     /* multiply by (p+1)^(e-1) */
                seq_t pk = 1;
                seq_t base = (seq_t)(p + 1);
                for (idx_t i = 1; i < e; ++i) {
                    pk *= base;
                }
                factor *= pk;
            }

            result *= factor;
        }
    }

    if (temp > 1) {           /* remaining prime factor with exponent 1 */
        result *= (seq_t)2;
    }

    return result;
}
