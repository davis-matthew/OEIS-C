/*
 * A359539  Number of partitions of n into at most 4 distinct positive
 *          Fibonacci numbers (with a single type of 1).
 * https://oeis.org/A359539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into at most 4 distinct positive Fibonacci numbers
   (with a single type of 1).  The implementation enumerates all subsets of
   size 1–4 of the Fibonacci numbers ≤ n and counts those whose sum equals n. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n == 0 has the empty partition */
    if (n == 0) {
        return (seq_t)1;
    }

    /* build list of distinct Fibonacci numbers ≤ n, starting with 1,2,3,5,... */
    uint64_t fibs[64];
    size_t nfibs = 0;
    fibs[nfibs++] = 1;          /* first Fibonacci number (unique) */
    uint64_t a = 1, b = 2;
    while (b <= (uint64_t)n && nfibs < sizeof(fibs)/sizeof(fibs[0])) {
        fibs[nfibs++] = b;
        uint64_t tmp = a + b;
        a = b;
        b = tmp;
    }

    seq_t count = 0;
    const uint64_t N = (uint64_t)n;

    /* one-element subsets */
    for (size_t i = 0; i < nfibs; ++i) {
        uint64_t s1 = fibs[i];
        if (s1 == N) {
            ++count;
        } else if (s1 < N) {
            /* two-element subsets */
            for (size_t j = i + 1; j < nfibs; ++j) {
                uint64_t s2 = s1 + fibs[j];
                if (s2 == N) {
                    ++count;
                } else if (s2 < N) {
                    /* three-element subsets */
                    for (size_t k = j + 1; k < nfibs; ++k) {
                        uint64_t s3 = s2 + fibs[k];
                        if (s3 == N) {
                            ++count;
                        } else if (s3 < N) {
                            /* four-element subsets */
                            for (size_t l = k + 1; l < nfibs; ++l) {
                                uint64_t s4 = s3 + fibs[l];
                                if (s4 == N) {
                                    ++count;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return count;
}
