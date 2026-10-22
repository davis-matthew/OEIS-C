/*
 * A364771  Order of the symplectic group of 4 X 4 matrices over Z_n.
 * https://oeis.org/A364771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{primes p|n} p^(10*v_p(n)-6)*(p^2-1)*(p^4-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    while (temp > 1) {
        uint64_t p = 0;

        /* find smallest prime factor */
        if (temp % 2 == 0) {
            p = 2;
        } else {
            for (uint64_t i = 3; i * i <= temp; i += 2) {
                if (temp % i == 0) {
                    p = i;
                    break;
                }
            }
        }
        if (p == 0) {          /* temp itself is prime */
            p = temp;
        }

        /* determine exponent e */
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* compute p^(10*e-6) * (p^2-1) * (p^4-1) */
        seq_t p_seq = (seq_t)p;
        seq_t p2 = p_seq * p_seq;           /* p^2 */
        seq_t p4 = p2 * p2;                 /* p^4 */

        uint64_t exp = 10 * e - 6;
        seq_t pow_val = 1;
        seq_t base = p_seq;
        while (exp > 0) {
            if (exp & 1) pow_val *= base;
            base *= base;
            exp >>= 1;
        }

        seq_t term = pow_val * (p2 - (seq_t)1) * (p4 - (seq_t)1);
        result *= term;
    }

    return result;
}
