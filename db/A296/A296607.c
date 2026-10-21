/*
 * A296607  a(n) = BarnesG(2*n).
 * https://oeis.org/A296607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = BarnesG(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Special case: a(0) = 0 as per OEIS */
    if (n == 0)
        return (seq_t)0;

    /* base value: a(1) = BarnesG(2) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Use the recurrence:
         A(n+1) = A(n) * (2n-1)! * (2n)!   for n >= 1
       We maintain (2n-1)! as fact_odd and (2n)! as fact_even. */
    seq_t A       = (seq_t)1;                 /* A(1)   */
    seq_t fact_odd  = (seq_t)1;                /* (2*1-1)!  = 1!  */
    seq_t fact_even = (seq_t)2;                /* (2*1)!    = 2!  */

    for (idx_t i = 1; i + 1 <= n; ++i)
    {
        /* Compute A(i+1) = A(i) * fact_odd * fact_even */
        seq_t t1 = seq_mul(A, fact_odd);
        A = seq_mul(t1, fact_even);

        /* Prepare (2(i+1)-1)! and (2(i+1))! for next iteration */
        uint64_t next_odd  = 2 * i + 1;   /* 2i+1  */
        uint64_t next_even = 2 * i + 2;   /* 2i+2  */

        seq_t next_fact_odd  = seq_mul(fact_even, (seq_t)next_odd);
        seq_t next_fact_even = seq_mul(next_fact_odd, (seq_t)next_even);

        fact_odd  = next_fact_odd;
        fact_even = next_fact_even;
    }

    return A;
}
