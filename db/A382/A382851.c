/*
 * A382851  a(n) = least number in row n of Pascal's triangle that exceeds
 *          every number in row n-1.
 * https://oeis.org/A382851
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least binomial coefficient C(n,k) that exceeds every
 *            binomial coefficient of row n‑1
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)          /* undefined for offset 2, any value is acceptable */
        return 0;

    /* Compute the maximum of row n‑1: M_prev = C(n-1, floor((n-1)/2)) */
    idx_t prev = n - 1;
    idx_t k_prev = prev >> 1;           /* floor((n-1)/2) */
    seq_t M_prev = 1;
    for (idx_t i = 1; i <= k_prev; ++i) {
        /* M_prev = M_prev * (prev - i + 1) / i  */
        M_prev = M_prev * (seq_t)(prev - i + 1) / (seq_t)i;
    }

    /* Find the smallest k such that C(n,k) > M_prev */
    seq_t val = 1;                       /* C(n,0) */
    for (idx_t k = 1; k <= n; ++k) {
        val = val * (seq_t)(n - k + 1) / (seq_t)k;
        if (val > M_prev)
            return val;
    }

    return val;   /* fallback: never reached for n >= 2 */
}
