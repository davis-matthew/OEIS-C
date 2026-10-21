/*
 * A292944  a(n) = A292272(A004754(n)) - 2*A053644(n).
 * https://oeis.org/A292944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A292272(A004754(n)) - 2*A053644(n) Implementation
 *            directly follows the bit‑masking description: • clear the most
 *            significant 1‑bit, • keep the second most significant 1‑bit
 *            regardless of its left neighbour, • keep any other 1‑bit only if
 *            its immediate left neighbour (the next higher bit) i...
 * Range:     u8 n <= 767, u16 n <= 9097, u32 n <= 9097, u64 n <= 9097,
 *            u128 n <= 9097, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: position of the most significant set bit (msb) for a non‑zero value.
   Returns 0 for n == 1. */
static inline idx_t msb_idx(idx_t n)
{
    idx_t pos = 0;
    while (n >>= 1) {
        ++pos;
    }
    return pos;
}

seq_t a(idx_t n)
{
    /* n = 0 or n = 1 produce 0 */
    if (n <= 1)
        return (seq_t)0;

    /* most significant bit position */
    idx_t m = msb_idx(n);

    /* remove msb to find the second msb */
    idx_t mask_msb = ((idx_t)1 << m);
    idx_t temp = n & ~mask_msb;          /* clear the msb */
    if (temp == 0)                       /* no second msb */
        return (seq_t)0;

    /* second msb position */
    idx_t s = msb_idx(temp);

    seq_t result = (seq_t)0;

    /* iterate over bits below the msb */
    for (idx_t i = 0; i < m; ++i) {
        if (((n >> i) & 1u) == 0)
            continue;                    /* bit not set */

        if (i == s) {
            result |= ((seq_t)1 << i);   /* second msb is always kept */
        } else {
            /* keep only if the immediate left neighbour bit is 0 */
            idx_t left = i + 1;           /* left neighbour (more significant) */
            if (((n >> left) & 1u) == 0) {
                result |= ((seq_t)1 << i);
            }
            /* otherwise the bit is cleared (do nothing) */
        }
    }

    return result;
}
