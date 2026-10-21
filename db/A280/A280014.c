/*
 * A280014  Numbers m == +- 2 (mod 10) but not m == 2 (mod 6).
 * https://oeis.org/A280014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A280014
 * Formula: a(n+4) = a(n)+30
 * Initial terms (n = 1..4): 12, 18, 22, 28
 *
 * Implementation uses the periodic pattern: a(n) = base[(n-1) mod 4] + 30 * floor((n-1)/4)
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n == 0; safe value */

    static const seq_t base[4] = { (seq_t)12, (seq_t)18, (seq_t)22, (seq_t)28 };

    idx_t idx      = n - 1;          /* zero‑based index */
    idx_t period   = idx / 4;        /* how many full 30‑step periods */
    idx_t offset   = idx % 4;        /* position within the 4‑term block */

    return base[(size_t)offset] + (seq_t)period * (seq_t)30;
}
