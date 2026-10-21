/*
 * A277994  Number of unordered integer pairs of the form {k | n, (k + 2^m) |
 *          n}, where k >= 1, m >= 0.
 * https://oeis.org/A277994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered divisor pairs {k, k+2^m} of n where k
 *            ≥ 1 and m ≥ 0. The algorithm enumerates all divisors of n and
 *            counts the admissible pairs in O(d(n) log n) time (d(n) = number
 *            of divisors). All arithmetic uses 64‑bit unsigned integers; the
 *            result is cast to seq_t for return
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)          /* a(1) = 0 */
        return (seq_t)0;

    uint64_t N = (uint64_t)n;
    /* A safe upper bound for the number of divisors of a 64‑bit number
       is 1536, but for n ≤ 10 000 it is far smaller.  Allocate a modest
       fixed buffer. */
    uint64_t divs[1536];
    size_t divcnt = 0;

    /* Enumerate all positive divisors of N. */
    for (uint64_t i = 1; i <= N / i; ++i)
    {
        if (N % i == 0)
        {
            divs[divcnt++] = i;
            uint64_t j = N / i;
            if (j != i)
                divs[divcnt++] = j;
        }
    }

    seq_t count = (seq_t)0;

    /* For each divisor k, count admissible m. */
    for (size_t idx = 0; idx < divcnt; ++idx)
    {
        uint64_t k = divs[idx];
        uint64_t limit = N - k;            /* maximum allowed 2^m */
        if (limit < 1)
            continue;                      /* no positive power of two fits */

        for (uint64_t pow2 = 1; pow2 <= limit; pow2 <<= 1)
        {
            uint64_t d2 = k + pow2;
            if (N % d2 == 0)
                ++count;

            /* Prevent overflow of pow2 for extremely large N. */
            if (pow2 > (UINT64_MAX >> 1))
                break;
        }
    }

    return count;
}
