/*
 * A390790  Numbers k such that 2*k - (greatest prime < 2*k) > (least prime >
 *          2*k) - 2*k.
 * https://oeis.org/A390790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 2*k - (greatest prime < 2*k) > (least prime > 2*k) - 2*k.
   Computed by a simple search: for each candidate k the nearest primes
   surrounding 2*k are found by trial division using is_prime().                */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Find the greatest prime strictly less than x (x >= 2).
   For x <= 2 (only when 2*k == 2) we return 0, so that
   2*k - 0 > next_prime(2*k) - 2*k holds for k = 1.                */
static idx_t prev_prime(idx_t x)
{
    if (x <= 2)
        return 0;
    idx_t i = x - 1;
    while (true) {
        if (is_prime(i))
            return i;
        --i;
    }
}

/* Find the least prime strictly greater than x.                    */
static idx_t next_prime(idx_t x)
{
    idx_t i = x + 1;
    while (true) {
        if (is_prime(i))
            return i;
        ++i;
    }
}

/* Return the n-th element (n ≥ 1) of OEIS A390790. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many valid k have been found */
    idx_t k = 1;              /* candidate k */

    while (count < n) {
        idx_t two_k = 2 * k;
        idx_t p_prev = prev_prime(two_k);
        idx_t p_next = next_prime(two_k);

        /* left = 2*k - p_prev, right = p_next - 2*k */
        if ((two_k - p_prev) > (p_next - two_k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
    /* Should never reach here */
    return 0;
}
