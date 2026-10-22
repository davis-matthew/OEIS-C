/*
 * A323153  a(n) = 1 if n is either a prime or a perfect number, 0 otherwise.
 * https://oeis.org/A323153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is prime or perfect number, 0 otherwise
 * Range:     u8 n <= 9942, u16 n <= 9942, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 0 and 1 are neither prime nor perfect */
    if (n < 2)
        return (seq_t)0;

    if (is_prime(n))
        return (seq_t)1;

    /* Known even perfect numbers that fit into idx_t (uint64_t) */
    static const idx_t perfects[] = {
        6ULL,
        28ULL,
        496ULL,
        8128ULL,
        33550336ULL,
        8589869056ULL,
        137438691328ULL,
        2305843008139952128ULL
    };
    static const idx_t perfect_cnt = sizeof(perfects) / sizeof(perfects[0]);

    for (idx_t i = 0; i < perfect_cnt; ++i)
        if (n == perfects[i])
            return (seq_t)1;

    return (seq_t)0;
}
