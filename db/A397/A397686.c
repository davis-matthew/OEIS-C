/*
 * A397686  Multiplicative with a(p^e) = (e+1)*(e+2)*(2*e+1)/2.
 * https://oeis.org/A397686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} (e+1)(e+2)(2e+1)/2 (multiplicative)
 * Range:     u8 n <= 11, u16 n <= 1259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;

    /* Handle factor 2 separately */
    if (temp % 2 == 0) {
        idx_t e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        seq_t term = ((seq_t)(e + 1) * (e + 2) * (2 * e + 1)) / 2;
        result *= term;
    }

    /* Handle odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t term = ((seq_t)(e + 1) * (e + 2) * (2 * e + 1)) / 2;
            result *= term;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        seq_t term = ((seq_t)(1 + 1) * (1 + 2) * (2 * 1 + 1)) / 2; /* e = 1 */
        result *= term;
    }

    return result;
}
