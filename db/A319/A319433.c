/*
 * A319433  Take the Zeckendorf representation of n (A014417(n)), drop least
 *          significant bit, take the inverse Zeckendorf representation.
 * https://oeis.org/A319433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor((n+2)/phi)-1 (Zeckendorf: drop LSB, shift)
 * Range:     u8 n <= 349, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Computes a(n) from OEIS A319433 */
seq_t a(idx_t n)
{
    /* Fibonacci numbers F1=1, F2=2, ... stored with fib[1]..fib[last] */
    static seq_t fib[200];
    static size_t fib_len = 0;               /* number of valid entries (index of max <= n) */

    /* Initialize the table once */
    if (fib_len == 0) {
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)2;
        fib_len = 2;
    }

    /* Extend the table while the next Fibonacci number is <= n
       and no unsigned overflow occurs */
    while (fib_len + 1 < sizeof(fib)/sizeof(fib[0])) {
        seq_t next = fib[fib_len] + fib[fib_len-1];
        /* Detect unsigned overflow: wrapped addition results in a smaller
           value than one of the summands */
        if (next <= fib[fib_len] || next <= fib[fib_len-1])
            break;                /* overflow, stop extending */
        if ((seq_t)n < next)
            break;                /* next fib would be > n */
        fib_len++;
        fib[fib_len] = next;
    }

    /* For n = 0 or 1, a(n) = 0 directly */
    if (n <= 1)
        return (seq_t)0;

    /* Greedy Zeckendorf decomposition of n.
       idx_arr holds the indices of the selected Fibonacci numbers. */
    idx_t rem = n;
    idx_t idx_arr[128];
    int cnt = 0;

    for (int i = (int)fib_len; i >= 1; ) {
        if (fib[i] <= (seq_t)rem) {
            rem -= (idx_t)fib[i];
            idx_arr[cnt++] = (idx_t)i;
            i -= 2;               /* skip the next Fibonacci to avoid consecutive ones */
        } else {
            i -= 1;
        }
    }

    /* Compute a(n) by shifting: bits at position i become position i-1
       in the result (i.e., add fib[i-1] for each chosen i >= 2). */
    seq_t sum = (seq_t)0;
    for (int j = 0; j < cnt; ++j) {
        if (idx_arr[j] >= 2) {
            sum += fib[idx_arr[j]-1];
        }
    }

    return sum;
}
