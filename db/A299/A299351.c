/*
 * A299351  For x=n, iterate the map x -> Product_{k is a prime dividing x} (k
 *          + 1), a(n) is the number of steps to see a repeated term for the
 *          first time.
 * https://oeis.org/A299351
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of steps needed, starting from n, until the
 *            first repeated term occurs when iterating f(x) = product of (p +
 *            1) over all primes p dividing x
 * Range:     u8 n <= 352, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Start from n (converted to seq_t) */
    seq_t current = (seq_t)n;

    /* Store visited values; 256 is ample for this sequence (cycle lengths ≤ 10). */
    seq_t visited[256];
    size_t seen_cnt = 1;
    visited[0] = current;

    size_t step = 0;

    while (1) {
        ++step;

        /* Compute next value: f(current) */
        seq_t next = 1;
        {
            uint64_t ut = (uint64_t)current;   /* current assumed ≤ 2^64-1 in typical use */
            uint64_t prime[64];
            uint64_t exp[64];
            size_t cnt;
            factor_u64(ut, prime, exp, &cnt);

            for (size_t i = 0; i < cnt; ++i) {
                next *= (seq_t)(prime[i] + (seq_t)1);
            }
        }

        /* Check if next has appeared before */
        for (size_t i = 0; i < seen_cnt; ++i) {
            if (visited[i] == next) {
                return (seq_t)step;
            }
        }

        /* Record and continue */
        visited[seen_cnt++] = next;
        current = next;
    }
}
