/*
 * A339371  Number of partitions of n into an even number of Fibonacci parts
 *          (with a single type of 1).
 * https://oeis.org/A339371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an even number of
 *            Fibonacci parts (the part of size 1 is counted only once). The
 *            function uses dynamic‐programming with two parity tables
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1U;                    /* a(0) = 1 */

    /* Allocate dynamic arrays for even/odd part counts. */
    size_t sz = (size_t)(n + 1);
    seq_t *even = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *odd  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!even || !odd) {                    /* allocation failure */
        free(even);
        free(odd);
        return (seq_t)0U;
    }

    /* Initialise to zero and set the empty partition (0 parts, even). */
    for (size_t i = 0; i < sz; ++i) {
        even[i] = 0;
        odd[i]  = 0;
    }
    even[0] = 1;

    /* Generate the Fibonacci numbers up to n, counting the part 1 once. */
    idx_t fibs[64];
    size_t fcnt = 0;
    if (n >= 1) fibs[fcnt++] = 1ULL;          /* first 1 */
    if (n >= 2) {
        fibs[fcnt++] = 2ULL;                  /* second distinct Fibonacci value */
        idx_t a = 1ULL, b = 2ULL;
        while (1) {
            idx_t next = a + b;
            if (next > n || fcnt >= sizeof(fibs)/sizeof(fibs[0]))
                break;
            fibs[fcnt++] = next;
            a = b;
            b = next;
        }
    }

    /* Dynamic programming: for each Fibonacci coin, update
       the even/odd count tables. */
    for (size_t fi = 0; fi < fcnt; ++fi) {
        idx_t f = fibs[fi];
        for (idx_t s = f; s <= n; ++s) {
            seq_t e_old = even[s];
            seq_t o_old = odd[s];
            even[s] = e_old + odd[s - f];   /* new even comes from old even + odd with one coin added */
            odd[s]  = o_old + even[s - f];  /* new odd comes from old odd  + even with one coin added */
        }
    }

    seq_t result = even[(size_t)n];
    free(even);
    free(odd);
    return result;
}
