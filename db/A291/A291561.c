/*
 * A291561  Diagonal in triangle A291560: a(n) = -A291560(n+1, n) for n>=1.
 * https://oeis.org/A291561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (2n-3)!! * (2n+1)!! / 3 where m!! denotes the double
 *            factorial (product of all odd numbers ≤ m)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; we return 0 for n = 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t n_val = (seq_t)n;          /* hold n as seq_t for later use */

    /* (2n-3)!! : product of odd numbers from 1 to (2n-3).  The loop
       runs only for n ≥ 2, because (2n-3) is negative for n = 1.      */
    seq_t prod1 = 1;
    if (n >= 2) {
        /* 2*n safely fits into idx_t for n ≤ 10000. */
        idx_t upper = 2 * n - 3;
        for (idx_t k = 1; k <= upper; k += 2)
            prod1 *= (seq_t)k;
    }

    /* (2n+1)!! : product of odd numbers from 1 to (2n+1). */
    seq_t prod2 = prod1;             /* reuse prod1 as a starting point */
    /* Start at the first odd number above (2n-3). */
    for (idx_t k = 2 * n - 1; k <= 2 * n + 1; k += 2)
        prod2 *= (seq_t)k;

    /* Assemble the final result: n * prod1 * prod2 / 3. */
    seq_t result = n_val;
    result *= prod1;
    result *= prod2;
    result /= (seq_t)3;

    return result;
}
