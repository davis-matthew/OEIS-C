/*
 * A374117  a(n) = 1 if A328768(n) and A328845(n) are both multiples of 3,
 *          otherwise 0, where A328768 is the first primorial based variant
 *          and A328845 is the first Fibonacci-based variant of the arithmetic
 *          derivative, respectively.
 * https://oeis.org/A374117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A328768(n) and A328845(n) are both multiples of 3,
 *            otherwise 0
 * Range:     u8 n <= 9663, u16 n <= 9663, u32 n <= 9663, u64 n <= 9663,
 *            u128 n <= 9663, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* both derivatives of 0 are 0, a(0)=1 */

    idx_t orig = n;
    idx_t m = n;

    uint8_t sum_p = 0;   /* derivative mod 3 for primorial variant */
    uint8_t sum_f = 0;   /* derivative mod 3 for Fibonacci variant */

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }

            uint8_t e_mod = e % 3;
            uint8_t np_mod = (orig / p) % 3;

            /* primorial derivative: d_p(p) = product of primes ≤ p */
            uint8_t d_p_mod = (p == 2) ? 2 : 0;   /* 0 if p≥3 because includes 3 */

            /* Fibonacci derivative: d_f(p) = F_p  (mod 3) */
            uint8_t d_f_mod;
            switch ((unsigned)(p % 8)) {
                case 0: d_f_mod = 0; break;
                case 1: d_f_mod = 1; break;
                case 2: d_f_mod = 1; break;
                case 3: d_f_mod = 2; break;
                case 4: d_f_mod = 0; break;
                case 5: d_f_mod = 2; break;
                case 6: d_f_mod = 2; break;
                case 7: d_f_mod = 1; break;
                default: d_f_mod = 0;  /* unreachable */
            }

            uint8_t term_p = ((e_mod * d_p_mod) % 3 * np_mod) % 3;
            uint8_t term_f = ((e_mod * d_f_mod) % 3 * np_mod) % 3;

            sum_p = (sum_p + term_p) % 3;
            sum_f = (sum_f + term_f) % 3;
        }
    }

    if (m > 1) {                           /* remaining prime factor */
        idx_t p = m;
        uint8_t e_mod = 1;                 /* exponent is 1 */
        uint8_t np_mod = (orig / p) % 3;

        uint8_t d_p_mod = (p == 2) ? 2 : 0;

        uint8_t d_f_mod;
        switch ((unsigned)(p % 8)) {
            case 0: d_f_mod = 0; break;
            case 1: d_f_mod = 1; break;
            case 2: d_f_mod = 1; break;
            case 3: d_f_mod = 2; break;
            case 4: d_f_mod = 0; break;
            case 5: d_f_mod = 2; break;
            case 6: d_f_mod = 2; break;
            case 7: d_f_mod = 1; break;
            default: d_f_mod = 0;
        }

        uint8_t term_p = ((e_mod * d_p_mod) % 3 * np_mod) % 3;
        uint8_t term_f = ((e_mod * d_f_mod) % 3 * np_mod) % 3;

        sum_p = (sum_p + term_p) % 3;
        sum_f = (sum_f + term_f) % 3;
    }

    bool mult_p = (sum_p == 0);
    bool mult_f = (sum_f == 0);

    return (seq_t)(mult_p && mult_f ? 1 : 0);
}
