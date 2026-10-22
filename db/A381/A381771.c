/*
 * A381771  For any n > 0, a(n) is the least positive multiple of n whose
 *          factorial base expansion has digits in nonincreasing order; a(0) =
 *          0.
 * https://oeis.org/A381771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 646, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381771: a(n) = least positive multiple m·n whose factorial‑base
 * expansion has digits in nonincreasing order (search over m). */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle the trivial first term */
    if (n == 0) return 0;

    seq_t base = (seq_t)n;
    for (idx_t m = 1; ; ++m) {
        seq_t val = base * (seq_t)m;   /* candidate value m·n  */
        seq_t t = val;                 /* work on a copy for conversion */

        seq_t prev = 0;                /* previous (less significant) digit */
        bool ok = true;

        for (idx_t i = 1; t > 0; ++i) {
            seq_t d = t % (i + 1);     /* digit i (1‑based, least significant first) */
            if (i > 1 && d < prev) {  /* must be non‑increasing from most to least significant */
                ok = false;
                break;
            }
            prev = d;
            t /= (i + 1);
        }
        if (ok) return val;
    }
}
