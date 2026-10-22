/*
 * A333208  a(n) is the numerator of Sum_{i >= 0} 1/(Lucas(i)*Lucas(i+2n)),
 *          with Lucas(i) as defined in A000032.
 * https://oeis.org/A333208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of (1/Fibonacci(2n)) * * Σ_{i=1..n}
 *            1/(Lucas(2i-2)*Lucas(2i-1)), * with Lucas(0)=2, Lucas(1)=1 and
 *            Lucas(k)=Lucas(k−1)+Lucas(k−2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"   /* provides seq_t fib(idx_t n); */
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* Lucas numbers l_prev = L_{2i-2}, l_curr = L_{2i-1} */
    seq_t l_prev = 2;     /* L_0 */
    seq_t l_curr = 1;     /* L_1 */

    /* Sum as a rational fraction sum_num / sum_den */
    seq_t sum_num = 0;    /* numerator of partial sum */
    seq_t sum_den = 1;    /* denominator of partial sum */

    for (idx_t i = 1; i <= n; ++i) {
        /* current term denominator: L_{2i-2} * L_{2i-1} */
        seq_t term_den = l_prev * l_curr;            /* 1 / term_den */

        /* add 1/term_den to existing sum */
        seq_t new_num = sum_num * term_den + sum_den;   /* numerator addition */
        seq_t new_den = sum_den * term_den;             /* denominator multiplication */

        seq_t g = gcd(new_num, new_den);                /* reduce fraction */
        sum_num = new_num / g;
        sum_den = new_den / g;

        /* advance Lucas pair for next iteration */
        if (i < n) {
            seq_t next1 = l_prev + l_curr;   /* L_{2i} */
            seq_t next2 = l_curr + next1;    /* L_{2i+1} */
            l_prev = next1;
            l_curr = next2;
        }
    }

    /* multiply by 1/Fibonacci(2n) */
    seq_t fib_2n = fib(2 * n);                 /* F_{2n}  */
    seq_t final_num = sum_num;
    seq_t final_den = sum_den * fib_2n;
    seq_t gfinal = gcd(final_num, final_den);
    final_num = final_num / gfinal;     /* ensures the fraction is reduced */

    return final_num;
}
