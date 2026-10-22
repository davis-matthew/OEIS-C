/*
 * A357731  Number of partitions of n into 2 distinct positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A357731
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of distinct Fibonacci numbers *
 *            (1,2,3,5,8,13,…) that sum to n, where 1 occurs only once
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* No result is defined for n < 3, but the function may safely return 0. */
    if (n < 3U) return 0;

    /* Fibonacci numbers up to n.  Only one '1' is used. */
    seq_t fib[64];
    size_t fib_len = 0;

    fib[fib_len++] = (seq_t)1;           /* first Fibonacci number */

    if (n >= (idx_t)2)                   /* second Fibonacci number */
        fib[fib_len++] = (seq_t)2;

    /* Generate subsequent Fibonacci numbers until they exceed n. */
    while (fib_len >= 2) {
        seq_t next = fib[fib_len - 1] + fib[fib_len - 2];
        if (next > (seq_t)n) break;
        fib[fib_len++] = next;
    }

    /* Count unordered distinct pairs (f[i], f[j]) with i < j and f[i] + f[j] == n. */
    seq_t count = 0;
    for (size_t i = 0; i < fib_len; ++i) {
        seq_t f1 = fib[i];
        if (f1 >= (seq_t)n) break;          /* f1 must be strictly < n */

        seq_t target = (seq_t)n - f1;
        for (size_t j = i + 1; j < fib_len; ++j) {
            if (fib[j] == target) {
                ++count;
                break;                       /* each unordered pair counted once */
            }
            if (fib[j] > target) break;       /* remaining fib numbers are larger */
        }
    }

    return count;
}
