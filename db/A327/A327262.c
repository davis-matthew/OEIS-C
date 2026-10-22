/*
 * A327262  a(n) is the sum of all parts of all partitions of n into
 *          consecutive parts that differ by 4.
 * https://oeis.org/A327262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (number of partitions of n into consecutive * parts
 *            differing by 4)
 * Range:     u8 n <= 59, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the nth term of sequence A327262.  The sequence is defined for
 * n ≥ 1; the function returns 0 for n == 0 so that it is safe to call
 * with arbitrary arguments.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* never requested by the test harness */

    seq_t count = 0;              /* number of admissible partitions */

    /* For each candidate length k we test whether n can be written as a
     * sum of k consecutive integers differing by 4, i.e. whether there
     * exist integers a≥1 and a_k = a-4(k-1) such that
     *     n = k * (a + a_k)/2
     *   => 2n = k * (2a - 4(k-1))
     *   => a = (n + 2k(k-1))/k
     *
     * The maximum possible k satisfies k(2k-1) ≤ n; we iterate k until
     * this inequality fails.  All arithmetic that could overflow
     * 64‑bit is performed in unsigned __int128, which GCC and Clang
     * support as an extension (common in the competition environment). */
    for (idx_t k = 1; ; ++k)
    {
        /* Stop when k*(2*k-1) > n.  Perform the comparison in 128 bits
         * to avoid overflow for large n. */
        unsigned __int128 t = (unsigned __int128)k * ((unsigned __int128)2 * k - 1);
        if (t > n)
            break;

        /* Compute numerator: n + 2*k*(k-1).  Use 128 bits to avoid overflow. */
        unsigned __int128 num = (unsigned __int128)n + (unsigned __int128)2 * k * (k - 1);

        /* a must be an integer, so num must be divisible by k. */
        if (num % k != 0)
            continue;

        /* Compute the largest part a = num / k. */
        idx_t a_part = (idx_t)(num / k);

        /* The smallest part is a_part - 4*(k-1); it must be ≥ 1.  This is
         * equivalent to a_part ≥ 4*(k-1)+1. */
        if (a_part >= 4 * (k - 1) + 1)
            count++;
    }

    /* The required value is n times the number of partitions. */
    return (seq_t)n * count;
}
