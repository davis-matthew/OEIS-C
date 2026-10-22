/*
 * A320986  Fibonacci rabbit sequence number n coded in base four.
 * https://oeis.org/A320986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, * a(n) = a(n-1) * 4^F(n-1) + a(n-2) for n >=
 *            2, * where F(k) is the k-th Fibonacci number * (F(0)=0, F(1)=1).
 *            The index n starts at 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Fibonacci numbers needed as exponents.
       We maintain fib_prev = F(i-2), fib_curr = F(i-1).  */
    idx_t fib_prev = 0;          /* F(0)  */
    idx_t fib_curr = 1;          /* F(1)  */

    seq_t a_nm2 = (seq_t)0;      /* a(n-2)   */
    seq_t a_nm1 = (seq_t)1;      /* a(n-1)   */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) = a_nm1 * 4^fib_curr + a_nm2 */
        seq_t pow4   = pow_int((seq_t)4, fib_curr);
        seq_t a_n    = a_nm1 * pow4 + a_nm2;

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;

        /* Advance Fibonacci numbers for next exponent */
        idx_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    return a_nm1;
}
