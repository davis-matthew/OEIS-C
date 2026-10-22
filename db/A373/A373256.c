/*
 * A373256  a(n) = 1 if A003415(n) == -1 (mod 3), otherwise 0, where A003415
 *          is the arithmetic derivative.
 * https://oeis.org/A373256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative of n is congruent to -1
 *            modulo 3, else 0
 * Range:     u8 n <= 9654, u16 n <= 9654, u32 n <= 9654, u64 n <= 9654,
 *            u128 n <= 9654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle n = 0 and n = 1 – their derivatives are 0 */
    if (n < 2)
        return (seq_t)0;

    uint64_t N   = (uint64_t)n;  /* principal integer value of n    */
    uint64_t rem = N;            /* value to be factored             */
    seq_t sum_mod = 0;           /* running sum modulo 3            */

    /* trial division up to sqrt(rem); comparison is safe from overflow */
    for (uint64_t p = 2; p <= rem / p; ++p) {
        uint64_t exp = 0;
        while (rem % p == 0) {
            ++exp;
            rem /= p;
        }
        if (exp == 0)
            continue;

        if (p == 3) {
            /* only exponent 1 contributes (n/3) mod 3; higher exponents give 0 */
            if (exp == 1) {
                seq_t term = (seq_t)((N / 3) % 3);
                sum_mod = (sum_mod + term) % 3;
            }
        } else {
            seq_t term = (seq_t)exp * (seq_t)((N / p) % 3);
            term %= 3;
            sum_mod = (sum_mod + term) % 3;
        }
    }

    /* any remaining prime factor (exponent 1) */
    if (rem > 1) {
        uint64_t p = rem;
        if (p == 3) {
            seq_t term = (seq_t)((N / 3) % 3);
            sum_mod = (sum_mod + term) % 3;
        } else {
            seq_t term = (seq_t)((N / p) % 3);  /* exp == 1 */
            term %= 3;
            sum_mod = (sum_mod + term) % 3;
        }
    }

    /* return 1 if d(n) ≡ -1 (i.e., 2) modulo 3; otherwise 0 */
    return (sum_mod % 3 == 2) ? (seq_t)1 : (seq_t)0;
}
