/*
 * A379104  a(n) = third elementary symmetric function of the first n distinct
 *          Fibonacci numbers.
 * https://oeis.org/A379104
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = elementary symmetric sum e₃ of the first n distinct
 *            positive Fibonacci * numbers 1,2,3,5,8,13,… (so a(3)=6, a(4)=61,
 *            a(5)=389,…)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* defined only for n ≥ 3 */

    seq_t e1 = 0, e2 = 0, e3 = 0; /* sums e₁, e₂, e₃ */
    idx_t count = 0;              /* how many distinct fib numbers seen */

    /* start with the first duplicate Fibonacci number (value 1) */
    seq_t prev = 1, curr = 1;     /* prev = F(k), curr = F(k+1) */
    seq_t last = 0;               /* last distinct Fibonacci value seen */

    while (count < n) {
        seq_t f = prev;           /* current Fibonacci value */

        if (f > last) {           /* a new distinct value */
            last = f;
            e3 += f * e2;
            e2 += f * e1;
            e1 += f;
            ++count;
        }

        /* advance to the next Fibonacci number */
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }

    return e3;                    /* third elementary symmetric function */
}
