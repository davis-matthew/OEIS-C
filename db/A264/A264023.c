/*
 * A264023  Suppose k is the concatenation of two integers, x and y: k =
 *          concat(x,y); a(n) is the smallest value of k such that x+y =
 *          prime(n).
 * https://oeis.org/A264023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = min{ concat(x,y) | x + y = prime(n) }
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   concat_parts, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the smallest number k formed by concatenating the
 * integers x and y such that x + y equals the n-th prime.        */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime (1‑indexed). */
    uint32_t p = get_prime(n);

    seq_t best = 0;
    bool first = true;

    for (idx_t x = 1; x < (idx_t)p; ++x) {
        idx_t y = (idx_t)p - x;
        seq_t k = concat_parts(x, y);
        if (first || k < best) {
            best = k;
            first = false;
        }
    }
    return best;
}
