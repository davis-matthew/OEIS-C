/*
 * A296591  a(n) = Product_{k=0..n} (n + k)!.
 * https://oeis.org/A296591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (n + k)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the recurrence
   a(0) = 1
   a(n) = a(n-1) * ((2n-1)! * (2n)!) / (n-1)!
   All intermediate values are exact integer operations. */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* Result so far; a(0) = 1 */
    seq_t result = (seq_t)1;

    /* Current factorial values:
       fact_n_minus1  = (n-1)!   (for current n)
       fact_2n_minus1 = (2n-1)! (for current n)
       fact_2n        = (2n)!   (for current n)     */
    seq_t fact_n_minus1 = (seq_t)1;            /* (0)!   */
    seq_t fact_2n_minus1 = (seq_t)1;          /* (1)!   */
    seq_t fact_2n = fact_2n_minus1 * (seq_t)2;/* (2)!   */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute ratio = (fact_2n_minus1 * fact_2n) / fact_n_minus1 */
        seq_t ratio = fact_2n_minus1 * fact_2n;
        ratio = ratio / fact_n_minus1;

        /* Update result */
        result = result * ratio;

        /* Prepare factorials for the next iteration (i -> i+1) */
        /* Update (i)! from (i-1)! */
        fact_n_minus1 = fact_n_minus1 * (seq_t)i;

        /* Compute factorial(2i+1) and factorial(2i+2) */
        seq_t new_fact_2n_minus1 = fact_2n * (seq_t)(2 * i + 1);
        fact_2n = new_fact_2n_minus1 * (seq_t)(2 * i + 2);
        fact_2n_minus1 = new_fact_2n_minus1;
    }

    return result;
}
