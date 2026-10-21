/*
 * A241425  Largest number k > 0 such that n + k! and n - k! are both prime,
 *          or 0 if no such k exists.
 * https://oeis.org/A241425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Largest k such that n + k! and n - k! are both prime.
   If no such k exists, return 0. */
#include <stddef.h>    /* for size_t */
#include <stdbool.h>   /* for bool */

#include "../utils/is_prime.h"

/* idx_t and seq_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* n is the 1‑based index; no special handling below the offset. */

    /* Compute all k! that are ≤ n and store them. */
    idx_t fact = 1;
    const idx_t MAX_K = 20;          /* more than enough for n ≤ 10000 */
    idx_t facts[MAX_K];
    size_t kcnt = 0;

    for (idx_t k = 1; k <= MAX_K; ++k) {
        fact = fact * k;              /* k! = (k-1)! * k */
        if (fact > n) break;          /* k! would be too large */
        facts[kcnt++] = fact;
    }

    /* Search for the largest k with both n±k! prime. */
    for (size_t i = kcnt; i > 0; --i) {
        idx_t f  = facts[i - 1];
        idx_t sum = n + f;
        idx_t diff = n - f;

        if (is_prime(sum) && is_prime(diff))
            return (seq_t)i;          /* i corresponds to k */
    }

    return 0;                          /* no suitable k */
}
