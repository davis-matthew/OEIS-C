/*
 * A304834  a(n) = 36*n^2 - 8*n - 2 (n >=1).
 * https://oeis.org/A304834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^2 - 8*n - 2
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return a(n) for n >= 1 using 64‑bit indices and arbitrary‑precision values.
   The computation is performed with an intermediate unsigned __int128 when
   available to avoid overflow for seq_t = uint64_t.  */
seq_t a(idx_t n) {
    /* std::uint64_t is guaranteed for idx_t, use a larger type for safety */
    unsigned __int128 t = (unsigned __int128)n * n * 36;
    t -= (unsigned __int128)8 * n;
    t -= 2;
    return (seq_t)t;
}
