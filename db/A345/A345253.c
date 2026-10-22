/*
 * A345253  Maximal Fibonacci tree: Arrangement of the positive integers as
 *          labels of a complete binary tree.
 * https://oeis.org/A345253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   finv_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from A345253 definition:
   a(1) = 1;
   for n > 1:
      let p = n / 2;
      if n is even:   a(n) = a(p) + F(Finv(a(p)));
      else:           a(n) = a(p) + F(Finv(a(p)) + 1);
   where F(k) is the k‑th Fibonacci number (F(1)=1, F(2)=1)
   and Finv(x) is the largest k with F(k) ≤ x. */

#include <stdint.h>   /* for consistency, though idx_t/seq_t are provided */
#include "../utils/finv_value.h"

/* Compute the k‑th Fibonacci number (F(0)=0, F(1)=1). */
static seq_t fib_of(idx_t k)
{
    if (k == 0)
        return 0;
    seq_t a = 0;
    seq_t b = 1;
    for (idx_t i = 1; i < k; ++i)
    {
        seq_t t = a + b;
        a = b;
        b = t;
    }
    return b;
}

/* Recursive helper to compute a(n). */
static seq_t a_value(idx_t n)
{
    if (n <= 1)
        return 1;

    seq_t parent = a_value(n >> 1);          /* parent index */
    idx_t k = finv_value(parent);            /* k = Finv(parent) */

    if ((n & 1) == 0)                       /* left child */
    {
        return parent + fib_of(k);           /* F(Finv(parent)) */
    }
    else                                     /* right child */
    {
        return parent + fib_of(k + 1);       /* F(Finv(parent)+1) */
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    return a_value(n);
}
