/*
 * A333089  a(n) is the denominator of Sum_{i > 0}
 *          1/(Fibonacci(i)*Fibonacci(i+2n)).
 * https://oeis.org/A333089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (1/Fibonacci(2n)) * Sum_{0 < i <= n}
 *            1/(Fibonacci(2i-1) * Fibonacci(2i))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Return the n-th term of sequence A333089.  The function works for
   any build of seq_t: 8, 16, 32, 64, 128‑bit integers or an
   arbitrary‑precision integer type.  It handles calls in any order and
   never allocates an array of seq_t objects, thereby avoiding
   construction problems on builds where seq_t is a C++ class. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset guard */

    seq_t num = 0;          /* current numerator of the partial sum   */
    seq_t den = 1;          /* current denominator of the partial sum */

    /* Fibonacci state: fib[k-1] (f_prev) and fib[k] (f_cur).  We start
       with fib[0] = 0, fib[1] = 1.  current_index holds the index of
       f_cur.  */
    seq_t f_prev = 0;       /* fib[0]   */
    seq_t f_cur  = 1;       /* fib[1]   */
    idx_t current_index = 1;/* f_cur corresponds to index 1 */

    seq_t fib2n = 1;        /* will be set to fib[2n] after the loop */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute fib[2i-1] ------------------------------------ */
        while (current_index < (2 * i - 1)) {
            seq_t f_next = f_prev + f_cur;
            f_prev = f_cur;
            f_cur  = f_next;
            ++current_index;
        }
        seq_t f1 = f_cur;                     /* fib[2i-1] */

        /* compute fib[2i] -------------------------------------- */
        seq_t f_next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur  = f_next;
        ++current_index;
        seq_t f2 = f_cur;                     /* fib[2i]   */

        if (i == n) fib2n = f2;               /* store fib[2n] for later */

        /* add the term 1/(f1 * f2) to the running fraction */
        seq_t d  = f1 * f2;                   /* denominator of the new term */
        seq_t g  = gcd(den, d);
        seq_t mul1 = d / g;                   /* d/g  */
        seq_t mul2 = den / g;                 /* den/g */

        seq_t new_num = num * mul1 + mul2;
        seq_t new_den = den * mul1;
        seq_t h = gcd(new_num, new_den);
        num = new_num / h;
        den = new_den / h;
    }

    /* multiply the result by 1/Fibonacci(2n) and return the
       denominator of the final fraction. */
    seq_t g2 = gcd(num, fib2n);
    return den * (fib2n / g2);
}
