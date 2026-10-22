/*
 * A300815  Number of 3 division {a, b, c} (a < b < c) of n where b isn't a
 *          multiple of a, c isn't a multiple of a and c isn't a multiple of
 *          b.
 * https://oeis.org/A300815
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 917, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A300815: number of 3‑element sets {a,b,c} (a<b<c) of n
 * with the non‑divisibility constraints:
 *   b % a != 0,  c % a != 0,  c % b != 0
 *
 * Implementation directly evaluates the defining summation over all possible
 * triples.  For n ≥ 10 the answer is returned; for n < 10 the function
 * returns 0 (the values below the offset are irrelevant for the tests). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If n is below the offset, the value is undefined; return 0. */
    if (n < 10)
        return (seq_t)0;

    seq_t count = 0;
    /* Outer loop over a.  a can range from 1 up to floor(n/3). */
    for (idx_t a_val = 1; a_val <= n / 3; ++a_val) {
        /* For each a, b runs from a+1 up to floor((n-a)/2).  This ensures
         * c = n - a - b > b (strictly increasing). */
        for (idx_t b_val = a_val + 1; b_val <= (n - a_val) / 2; ++b_val) {
            idx_t c_val = n - a_val - b_val;   /* the third element */

            /* Apply the three non‑divisibility constraints. */
            if (b_val % a_val == 0) continue;
            if (c_val % a_val == 0) continue;
            if (c_val % b_val == 0) continue;

            /* All constraints satisfied; count the triple. */
            ++count;
        }
    }
    return count;
}
