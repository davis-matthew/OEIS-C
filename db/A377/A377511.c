/*
 * A377511  a(n) = number of iterations of x -> 2 x + 7 to reach a nonprime,
 *          starting with prime(n).
 * https://oeis.org/A377511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> 2x + 7 required to reach a
 *            nonprime, starting from the n‑th prime
 * Range:     u8 n <= 30, u16 n <= 3850, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

/* Return the n-th term of OEIS sequence A377511. */
seq_t a(idx_t n)
{
    /* Find the n-th prime. */
    idx_t p = 2;
    idx_t seen = 1;
    while (seen < n) {
        p++;
        if (is_prime(p))
            seen++;
    }

    /* Iterate x -> 2x + 7 until x is composite. */
    seq_t x = (seq_t)p;
    seq_t count = 0;
    while (is_prime((idx_t)x)) {
        x = 2 * x + 7;
        count++;
    }
    return count;
}
