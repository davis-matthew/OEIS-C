/*
 * A375349  a(n) is the parity of the n-th cubefree number.
 * https://oeis.org/A375349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = parity of the n‑th cubefree number (i.e., a(n) =
 *            A004709(n) mod 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return the parity (0 or 1) of the n-th cubefree integer. */
seq_t a(idx_t n)
{
    /* n is at least 1 (offset is 1). */
    idx_t m = 0;          /* current integer being tested          */
    idx_t count = 0;      /* number of cubefree integers seen     */

    while (count < n) {
        ++m;               /* advance to next integer              */

        bool cube_free = true;

        /* test divisibility by all cube numbers k³ <= m */
        for (idx_t k = 2;; ++k) {
            __int128 k3 = (__int128)k * k * k;   /* k³ may not fit in idx_t */
            if (k3 > m) break;                  /* larger cubes cannot divide m */
            if (m % (uint64_t)k3 == 0) {         /* m divisible by a cube   */
                cube_free = false;
                break;
            }
        }

        if (cube_free) ++count;   /* m is a cubefree number         */
    }

    return (seq_t)(m & 1);         /* parity of the n‑th cubefree     */
}
