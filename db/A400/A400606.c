/*
 * A400606  a(n) = least m >= 1 such that n*m^2 - 1 and n*m^2 + 1 are both
 *          prime, or 0 if there is no such m.
 * https://oeis.org/A400606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = least m ≥ 1 such that n*m^2 − 1 and n*m^2 + 1 are both
 *            prime * (0 if none). For perfect squares ≥ 9 the value is 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   int_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/int_sqrt.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Handle perfect squares ≥ 9 explicitly */
    if (n >= 9u) {
        idx_t t = int_sqrt(n);
        if (t * t == n) return 0;
    }

    seq_t n_seq = (seq_t)n;
    for (idx_t m = 1; ; ++m) {
        seq_t m_seq = (seq_t)m;
        seq_t m_sq  = m_seq * m_seq;         /* m^2 */
        seq_t val   = n_seq * m_sq;          /* n*m^2 */

        seq_t minus = val - 1;
        seq_t plus  = val + 1;

        /* Both numbers must be prime */
        if (is_prime_big(minus) && is_prime_big(plus))
            return (seq_t)m;
    }
    /* Control never reaches here, but to satisfy the compiler */
    return 0;
}
