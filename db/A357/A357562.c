/*
 * A357562  a(n) = n - 2*b(b(n)) for n >= 2, where b(n) = A356988(n).
 * https://oeis.org/A357562
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = distance to the nearest integer of the form
 *            2*Fibonacci(k). Implementation uses the Fibonacci sequence to
 *            find the two consecutive values 2*Fibonacci(k) ≤ n <
 *            2*Fibonacci(k+1) and returns the smaller of the two distances to
 *            these points
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/*  a(n) = distance to nearest 2*Fib(k)   (n >= 2)  */
seq_t a(idx_t n)
{
    /* static cache of Fibonacci numbers */
    static idx_t *fibs   = NULL;   /* Fibonacci numbers (including 0,1,1,2,…)   */
    static size_t  count = 0;      /* number of stored Fibonacci numbers   */
    static size_t  cap   = 0;      /* current capacity of the array        */

    /* initialise cache on first call – 8 entries are enough for n up to 2^64 */
    if (cap == 0) {
        cap   = 8;
        fibs  = (idx_t*)malloc(cap * sizeof(idx_t));
        fibs[0] = 0;
        fibs[1] = 1;
        count  = 2;
    }

    /* extend cache until 2*fibs[count‑1] > n   (the last Fib is > n/2) */
    while (((seq_t)2 * fibs[count - 1]) <= n) {
        if (count == cap) {                     /* grow array                */
            size_t newcap = cap * 2;
            if (newcap < 8) newcap = 8;
            idx_t *tmp = (idx_t*)realloc(fibs, newcap * sizeof(idx_t));
            if (!tmp) {                         /* allocation failure – return 0   */
                return (seq_t)0;
            }
            fibs = tmp;
            cap  = newcap;
        }
        fibs[count] = fibs[count - 1] + fibs[count - 2];
        count++;
    }

    /* locate the two consecutive Fibonacci multiples surrounding n   */
    idx_t k     = count - 2;                      /* 2*fibs[k] <= n  */
    seq_t base  = ((seq_t)2 * fibs[k]);           /* left end        */
    seq_t upper = ((seq_t)2 * fibs[k + 1]);       /* right end       */

    /* compute distances – both are non‑negative (n >= base, n < upper) */
    seq_t diff1 = (seq_t)n - base;
    seq_t diff2 = upper - (seq_t)n;

    return diff1 < diff2 ? diff1 : diff2;
}
