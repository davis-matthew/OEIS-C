/*
 * A380956  Position of first appearance of n in A380955 (sum of prime indices
 *          minus sum of distinct prime indices).
 * https://oeis.org/A380956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 for n >= 12; a(0..11) are pre‑computed
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    static const seq_t pre[12] = {
        1, 4, 8, 16, 27,
        64, 81, 256, 243, 529,
        729, 961
    };

    if (n < 12) return pre[n];

    /* get_prime is 1‑based; it returns the n‑th prime as a 32‑bit value. */
    uint32_t p = get_prime(n);
    return (seq_t)p * (seq_t)p;
}
