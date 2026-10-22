/*
 * A356388  a(n) is the product of all parts in negaFibonacci representation
 *          of -n.
 * https://oeis.org/A356388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 88, i32 n <= 609, i64 n <= 9236,
 *            i128 n <= 9236, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*,
  OEIS entry A356388.
  Formula: a(n) = product of all parts in negaFibonacci representation of -n.
  This implementation finds the unique representation of -n using
  negative-index Fibonacci numbers F(-k) (k ≥ 1) with no two consecutive indices,
  and multiplies the chosen terms.
*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Recursive search for the NeGafibonacci representation.
   rem  : target sum (can be negative),
   k    : highest index still allowed to use (no consecutive indices)
   vals : array of F(-i) values, 1‑based (size >= k+1)
   absSum: cumulative absolute sums of vals[1..i] for pruning.
*/
static seq_t
rec(long long rem, int k, const seq_t *vals, const long long *absSum)
{
    if (rem == 0)            /* empty product */
        return (seq_t)1;
    if (k == 0)              /* no numbers left */
        return (seq_t)0;
    if (llabs(rem) > absSum[k])  /* impossible regardless of choices */
        return (seq_t)0;

    /* Try including the k‑th term first (often leads to the solution). */
    long long newRem = rem - (long long)vals[k];
    seq_t sub = rec(newRem, k - 2, vals, absSum);
    if (sub != (seq_t)0)
        return vals[k] * sub;

    /* If inclusion didn't work, try skipping k. */
    return rec(rem, k - 1, vals, absSum);
}

seq_t a(idx_t n)
{
    /* a(0) is defined as the empty product 1. */
    if (n == 0)
        return (seq_t)1;

    /* Pre–compute Fibonacci numbers (positive indices). */
    /* We keep enough numbers so that the largest |F(-k)| needed
       is certainly larger than n.  For n ≤ 10000, k ≤ 23 suffices. */
    enum { MAX_K = 25 };          /* safety margin */
    seq_t fib[MAX_K + 1];
    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;
    fib[2] = (seq_t)1;
    int fibLen = 2;
    while (fibLen < MAX_K) {
        seq_t next = fib[fibLen] + fib[fibLen - 1];
        fib[++fibLen] = next;
    }

    /* Determine the index range we need.  We include all Fibonacci
       numbers up to the smallest index where F(k) > n + F(k‑1),
       plus a few more for safety. */
    int maxk = 2;
    while (maxk < fibLen &&
           (long long)fib[maxk] <= (long long)n + (long long)fib[maxk - 1])
        ++maxk;
    if (maxk > fibLen)
        maxk = fibLen;

    /* Build the array of values F(-k) = (-1)^(k+1) * F(k). */
    static seq_t vals[MAX_K + 1];
    for (int i = 1; i <= maxk; ++i) {
        seq_t v = fib[i];
        if ((i & 1) == 0)   /* even k gives a negative term */
            vals[i] = -v;
        else
            vals[i] =  v;
    }

    /* Pre‑compute cumulative absolute sums for pruning. */
    static long long absSum[MAX_K + 1];
    absSum[0] = 0;
    for (int i = 1; i <= maxk; ++i) {
        long long absVal = vals[i] >= 0 ? (long long)vals[i]
                                        : -(long long)vals[i];
        absSum[i] = absSum[i - 1] + absVal;
    }

    /* The target sum is -n. */
    long long target = -(long long)n;
    return rec(target, maxk, vals, absSum);
}
