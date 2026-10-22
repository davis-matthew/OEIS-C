/*
 * A329193  a(n) = floor(log_2(n^3)) = floor(3 log_2(n)).
 * https://oeis.org/A329193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log2(n^3)) = floor(3 * log2(n))
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

/* Compute floor(log2(n^3)) without floating‑point arithmetic.
   Uses a 128‑bit intermediate to avoid overflow for n up to about 10^6. */
seq_t a(idx_t n)
{
    /* Compute n^3 in a 128‑bit integer. */
    unsigned __int128 prod = (unsigned __int128)n * n;
    prod *= n;              /* prod = n^3 */

    /* Determine the floor of log2(prod).
       If the high 64 bits are non‑zero, use them; otherwise use the low 64 bits. */
    unsigned int log2p;
    if ((prod >> 64) != 0) {
        log2p = 64 + floor_log2_uint64((uint64_t)(prod >> 64));
    } else {
        log2p = floor_log2_uint64((uint64_t)prod);
    }

    return (seq_t)log2p;
}
