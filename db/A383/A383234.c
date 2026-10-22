/*
 * A383234  Expansion of e.g.f. f(x)^4 * log(f(x)), where f(x) = 1/(1 -
 *          5*x)^(1/5).
 * https://oeis.org/A383234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 0 * a(1) = 1 * a(n) = (10*n-7)*a(n-1) - (5*n-6)²*a(n-2)
 *            for n > 1 * * This is the recurrence given in the OEIS entry for
 *            * A383234. It is directly evaluated using only the * primitive
 *            arithmetic operators, which work for all * supported types
 *            (built‑in integer types and the * arbitrary‑p...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The build supplies the definitions of seq_t and idx_t. */
seq_t a(idx_t n)
{
    if (n == 0) { return (seq_t)0; }
    if (n == 1) { return (seq_t)1; }

    seq_t a_nm2 = (seq_t)0;  /* a(0) */
    seq_t a_nm1 = (seq_t)1;  /* a(1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* (10*i - 7) * a(i-1) */
        seq_t coeff1 = (seq_t)(10*i - 7);
        seq_t term1  = coeff1 * a_nm1;

        /* (5*i - 6)² * a(i-2) */
        seq_t m     = (seq_t)(5*i - 6);
        seq_t term2 = m * m * a_nm2;   /* m² * a_nm2 */

        a_n = term1 - term2;          /* recurrence */

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
