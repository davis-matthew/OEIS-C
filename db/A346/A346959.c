/*
 * A346959  Pentagonal pandigitals.
 * https://oeis.org/A346959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = nth pentagonal number that contains each decimal digit
 *            0‑9 at least once
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   is_pandigital
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_pandigital.h"

/* Return the nth pentagonal pandigital number (OEIS A346959). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t count = 0;
    idx_t k     = 1;
    seq_t p     = 1;         /* p(1) = 1 */

    for (;;) {
        if (is_pandigital(p)) {
            ++count;
            if (count == n) return p;
        }

        /* Compute next pentagonal number: p(k+1) = p(k) + 3*k + 1. */
        seq_t diff = (seq_t)3 * (seq_t)k + 1;
        p += diff;
        ++k;
    }
}
