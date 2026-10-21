/*
 * A260820  Nonnegative integers n such that n^3-3 is divisible by n-3.
 * https://oeis.org/A260820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Nonnegative integers n such that n^3-3 is divisible by n-3.
   Equivalent condition: (n-3) divides 24.  The terms are
   0,1,2,4,5,6,7,9,11,15,27 (11 terms total). */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache of all sequence terms (there are only 11) */
    static seq_t term[11];
    static idx_t len = 0;
    static bool init = false;

    if (!init) {
        /* generate all n with (n-3) | 24 (n≥0) */
        for (int d = -3; d <= 24; ++d) {   /* d = n-3 */
            if (d == 0) continue;          /* skip division by zero */
            if (24 % d == 0) {              /* d divides 24 */
                int n_val = d + 3;          /* recover n */
                if (n_val >= 0) {
                    term[len++] = (seq_t)n_val;
                }
            }
        }
        init = true; /* cache populated */
    }

    if (n == 0) return 0;          /* offset 1; index 0 unused */
    if (n <= len) return term[n - 1];
    return 0;                     /* undefined for n>11: return 0 */
}
