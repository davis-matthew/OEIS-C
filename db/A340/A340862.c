/*
 * A340862  Number of times the number n turns up in pseudo-Fibonacci
 *          sequences starting with [k, 1] (with k >= 1), excluding the
 *          starting terms.
 * https://oeis.org/A340862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ m ≥ 3 : Fib(m) ≤ n AND (n - Fib(m-1)) mod Fib(m-2) = 0
 *            }
 * Range:     u8 n <= 689, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Counts the number of occurrences of n in all pseudo-Fibonacci
   sequences starting with [k,1] (k ≥ 1), excluding the starting
   terms.  Implements the explicit test derived from
   a_m = Fib(m-2)*k + Fib(m-1).  */
seq_t a(idx_t n)
{
    if (n < 2)                 /* a(1) = 0, a(0) not used */
        return 0;

    seq_t count = 0;
    seq_t sn = (seq_t)n;       /* cast once for all arithmetic */

    /* Fibonacci numbers: Fib(1)=1, Fib(2)=1 */
    seq_t f_m_minus_2 = 1;     /* F(m-2) */
    seq_t f_m_minus_1 = 1;     /* F(m-1) */

    /* m starts at 3; loop while F(m)=f_m_minus_2+f_m_minus_1 ≤ n   */
    while (f_m_minus_2 + f_m_minus_1 <= sn) {
        /* n ≥ f_m_minus_1 + f_m_minus_2 guarantees k ≥ 1.  */
        seq_t diff = sn - f_m_minus_1;
        if ((diff % f_m_minus_2) == 0)
            count += 1;

        /* advance fibonacci: next = f_m_minus_1 + f_m_minus_2 */
        seq_t next = f_m_minus_2 + f_m_minus_1;
        f_m_minus_2 = f_m_minus_1;
        f_m_minus_1 = next;
    }

    return count;
}
