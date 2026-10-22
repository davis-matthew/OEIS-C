/*
 * A357732  Number of partitions of n into 3 distinct positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A357732
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 distinct positive
 *            Fibonacci numbers (1,2,3,5,8,…)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Build the Fibonacci list once.  The sequence starts 1,2,3,5,8,…,
       i.e. one “1”, then normal Fibonacci numbers. */
    static uint64_t fib[93];          /* fits 64‑bit, enough for all n up to >10^19 */
    static size_t fibcnt = 0;
    static bool built = false;

    if (!built) {
        fib[0] = 1;
        fib[1] = 2;
        fibcnt = 2;
        while (1) {
            uint64_t a = fib[fibcnt - 2];
            uint64_t b = fib[fibcnt - 1];
            if (UINT64_MAX - a < b)   /* would overflow */
                break;
            fib[fibcnt++] = a + b;
        }
        built = true;
    }

    if (n < 6)               /* offset 6: a(6) is the first term */
        return 0;

    seq_t count = 0;

    for (size_t i = 0; i + 2 < fibcnt; ++i) {
        uint64_t fi = fib[i];
        for (size_t j = i + 1; j + 1 < fibcnt; ++j) {
            uint64_t fsum2 = fi + fib[j];
            if (fsum2 > n)
                break;
            for (size_t k = j + 1; k < fibcnt; ++k) {
                seq_t sum = (seq_t)fsum2 + (seq_t)fib[k];
                if (sum == (seq_t)n)
                    ++count;
                else if (sum > (seq_t)n)
                    break;
            }
        }
    }

    return count;
}
