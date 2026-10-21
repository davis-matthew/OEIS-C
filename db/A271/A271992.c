/*
 * A271992  g_n(16) where g is the weak Goodstein function defined in A266202.
 * https://oeis.org/A271992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weak Goodstein function g_n(16) (OEIS A271992).
 *
 * The sequence is defined as follows:
 *  a(0) = 16.
 *  For n ≥ 1, let m = a(n‑1).  Let b = n+1 (i.e., b starts at 2 and
 *  increments by 1 at each step).  Express m in base b:
 *      m = Σ d_i · b^i  with 0 ≤ d_i < b.
 *  Evaluate the same digit expansion in the next base (b+1):
 *      m' = Σ d_i · (b+1)^i .
 *  Then a(n) = m' − 1.
 *
 * This implements that rule exactly using only integer arithmetic
 * over the types seq_t (the value type) and idx_t (the index type).
 * The code works for any unsigned integer seq_t, including arbitrary‑precision
 * types provided by the build (e.g., boost::multiprecision::cpp_int).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial value */
    seq_t x = (seq_t)16;
    if (n == 0)
        return x;

    /* starting base is 2, and we increment it each step */
    idx_t base = 2;

    for (idx_t step = 1; step <= n; ++step) {
        seq_t val = x;
        seq_t next = 0;
        seq_t mul = 1;                     /* (base+1)^position */
        seq_t base_inc = (seq_t)(base + 1); /* next base */

        /* decompose val in base 'base' and evaluate in base_inc */
        while (val > 0) {
            seq_t digit = val % (seq_t)base;
            next += digit * mul;
            mul *= base_inc;
            val /= (seq_t)base;
        }

        /* subtract 1 as per the definition */
        next -= 1;

        /* prepare for next iteration */
        x = next;
        ++base;
    }

    return x;
}
