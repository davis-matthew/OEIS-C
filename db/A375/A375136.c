/*
 * A375136  Number of maximal strictly increasing runs in the weakly
 *          increasing prime factors of n.
 * https://oeis.org/A375136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of adjacent equal prime factors of n (n > 1);
 *            a(1) = 0
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case for n = 1 */
    if (n == 1) {
        return 0;
    }

    idx_t count_adjacent_equal = 0;   /* Count of adjacent equal prime factors */
    idx_t prev_prime = 0;             /* The previous prime factor processed */
    idx_t m = n;                      /* Working copy of n for factorisation */

    /* Factor out 2 first for efficiency */
    if (m % 2 == 0) {
        while (m % 2 == 0) {
            m /= 2;
            if (prev_prime == 2) {
                ++count_adjacent_equal;
            }
            prev_prime = 2;
        }
    }

    /* Factor out odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            while (m % p == 0) {
                m /= p;
                if (prev_prime == p) {
                    ++count_adjacent_equal;
                }
                prev_prime = p;
            }
        }
    }

    /* If anything remains it is a prime larger than sqrt(original n) */
    if (m > 1) {
        /* m is now the last prime factor */
        if (prev_prime == m) {
            ++count_adjacent_equal;
        }
    }

    /* For n > 1 the sequence value is 1 + number of adjacent equal factors. */
    return (seq_t)count_adjacent_equal + (seq_t)1;
}
