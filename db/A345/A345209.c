/*
 * A345209  Number of Petrie polygons on the regular triangular map
 *          corresponding to the principal congruence subgroup Gamma(n) of the
 *          modular group.
 * https://oeis.org/A345209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = A001766(n)/A301759(n) for n >= 3; a(1) = a(2) = 1.
 *            A001766(n) = (n^3/2)*Product_{p|n}(1-1/p^2); A301759(n) =
 *            semiperiod of Fibonacci mod n
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 905 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A001766_a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    seq_t result = (seq_t)1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            seq_t pp = (seq_t)p;
            result = result * (pp * pp - 1);
            temp /= p;
            while (temp % p == 0) {
                temp /= p;
                result = result * pp * pp * pp;
            }
            result = result * pp;
        }
    }
    if (temp > 1) {
        seq_t pp = (seq_t)temp;
        result = result * (pp * pp - 1) * pp;
    }
    return result / 2;
}

static idx_t A301759_a(idx_t n) {
    if (n <= 1) return 1;
    idx_t a_val = 0;
    idx_t b_val = 1 % n;
    for (idx_t k = 1; ; ++k) {
        idx_t c = (idx_t)(((unsigned __int128)a_val + b_val) % n);
        a_val = b_val;
        b_val = c;
        if (a_val == 0 && (b_val == 1 || b_val == n - 1))
            return k;
    }
}

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)1;
    return A001766_a(n) / (seq_t)A301759_a(n);
}
