/*
 * A309017  Numbers that divide the sum of the digits of their cubes.
 * https://oeis.org/A309017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, full, fini
 * Formula:   a(n) = the nth integer n that divides the sum of the digits of
 *            n³ (search)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Pre‑computed list of the 9 known terms.  The list is filled lazily on
       the first call.  It is guaranteed that no further terms exist for
       n ≥ 1000, because for any m ≥ 1000 the cube m³ has at most 10 digits,
       so its digit sum ≤ 90 < m, hence m cannot divide it.  */
    static seq_t terms[20];
    static size_t term_count = 0;

    if (term_count == 0) {
        for (idx_t i = 1; i < 1000; ++i) {
            seq_t v = (seq_t)i;
            seq_t cube = v * v * v;
            seq_t s = digit_sum(cube);
            if ((s % (seq_t)i) == 0) {
                terms[term_count++] = v;
                if (term_count >= sizeof(terms)/sizeof(terms[0])) {
                    break; /* safety – only 9 terms should be found */
                }
            }
        }
    }

    if (n == 0 || (idx_t)n > term_count) {
        return (seq_t)0;   /* sequence undefined beyond the 9th term */
    }
    return terms[n - 1];
}
