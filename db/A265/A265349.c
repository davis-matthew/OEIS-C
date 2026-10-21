/*
 * A265349  Numbers in whose factorial base representation (A007623) no digit
 *          > 0 occurs more than once.
 * https://oeis.org/A265349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 4551, u32 n <= 9580, u64 n <= 9580,
 *            u128 n <= 9580, big any n
 * Helpers:   factorial_base_unique
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generating A265349 by brute‑force search: numbers whose
   factorial‑base representation has no repeated non‑zero digits. */
#include <stdbool.h>
#include "../utils/factorial_base_unique.h"

/* Return the n‑th term of A265349 (0‑indexed). */
seq_t a(idx_t n)
{
    idx_t count = 0;   /* number of valid terms found */
    seq_t v = 0;       /* current integer to test */

    while (count <= n) {
        if (factorial_base_unique(v)) {
            if (count == n) return v;
            count++;
        }
        v++;            /* test next integer */
    }
    /* control should never reach here */
    return 0;
}
