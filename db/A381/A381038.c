/*
 * A381038  Coefficients of the first Mock Eisenstein series associated to
 *          partition ranks.
 * https://oeis.org/A381038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m|n} [ (2*(n/m) - 3*m)*[3*m <= 2*(n/m)-1] - ( (n/m)
 *            - 6*m)*[6*m <= (n/m)-1] ]
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9686 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t contribution(idx_t m, idx_t n)
{
    seq_t add = 0, sub = 0;
    if (3*m <= 2*n - 1) {
        add = 2*(seq_t)n - 3*(seq_t)m;
    }
    if (6*m <= n - 1) {
        sub = (seq_t)(n - 6*m);
    }
    /* cast to signed to allow subtraction, then back to unsigned */
    long long delta = (long long)add - (long long)sub;
    return (seq_t)delta;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t m = 1; m*m <= n; ++m) {
        if (n % m == 0) {
            idx_t d1 = m;
            idx_t d2 = n / m;
            sum += contribution(d1, d2);
            if (d1 != d2) {
                sum += contribution(d2, d1);
            }
        }
    }
    return sum;
}
