/*
 * A370899  Partial alternating sums of the unitary totient function
 *          (A047994).
 * https://oeis.org/A370899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * A047994(k)
 * Range:     i8 n <= 40, i16 n <= 678, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9810 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where A047994(k) = ∏_{p^e‖k} (p^e - 1)          */

#include <stdint.h>

static inline seq_t _phi_star(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t power = 1;
            while (m % p == 0) {
                m /= p;
                power *= (seq_t)p;
            }
            result *= (power - 1);
        }
    }
    if (m > 1) {
        result *= ((seq_t)m - 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t v = _phi_star(k);
        if (k & 1) {
            sum += v;
        } else {
            sum -= v;
        }
    }
    return sum;
}
