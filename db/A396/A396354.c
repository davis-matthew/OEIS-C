/*
 * A396354  Number of unsatisfiable 3-SAT formulas with n variables and 4
 *          clauses in the multiset clause model.
 * https://oeis.org/A396354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*binomial(n,1)+811*binomial(n,2)+7178*binomial(n,3)+ *
 *            16400*binomial(n,4)+22000*binomial(n,5)+15360*binomial(n,6)+ *
 *            4480*binomial(n,7). The value is defined for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 574, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdbool.h>

/* External linkage: compute the OEIS sequence A396354. */
seq_t a(idx_t n)
{
    /* Below the offset the behaviour is irrelevant; return 0 for safety. */
    if (n == 0)
        return 0;

    seq_t result = 0;

    /* Compute the seven binomial contributions.  All intermediate results
       use the seq_t type, so the calculation is exact for arbitrary‑precision
       builds and safe from overflow for the intended seq_t width. */
    result += (seq_t)10 * binomial(n, 1);
    result += (seq_t)811 * binomial(n, 2);
    result += (seq_t)7178 * binomial(n, 3);
    result += (seq_t)16400 * binomial(n, 4);
    result += (seq_t)22000 * binomial(n, 5);
    result += (seq_t)15360 * binomial(n, 6);
    result += (seq_t)4480 * binomial(n, 7);

    return result;
}
