/*
 * A286920  Triangle read by rows: T(n,m) is the number of pattern classes in
 *          the (n,m)-rectangular grid with 9 colors and n>=m, two patterns
 *          are in the same class if one of them can be obtained by a
 *          reflection or 180-degree rotation of the other.
 * https://oeis.org/A286920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangular sequence A286920. For an (n,m)-rectangular grid with
 *            n ≥ m, the number of pattern classes under reflection or
 *            180‑degree rotation is calculated by Burnside’s lemma. This
 *            implementation uses the derived fixed‑coloring formulas and
 *            handles all cases correctly for arbitrary index n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row r and column c such that n = r*(r+1)/2 + c
       (each row r has r+1 entries, c ranges from 0 to r). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    idx_t c = n - r * (r + 1) / 2;

    idx_t nm = r * c;        /* total number of cells */

    /* Total colorings (identity). */
    seq_t t_identity = pow_uint(9, nm);

    seq_t sum = t_identity;
    int count = 1;  /* identity always present */

    /* Vertical reflection is distinct only when more than one column. */
    if (c > 1) {
        seq_t v;
        if (c % 2 == 0) {
            v = pow_uint(9, nm / 2);
        } else {
            v = pow_uint(9, (nm + r) / 2);
        }
        sum += v;
        count++;
    }

    /* Horizontal reflection is distinct only when more than one row. */
    if (r > 1) {
        seq_t h;
        if (r % 2 == 0) {
            h = pow_uint(9, nm / 2);
        } else {
            h = pow_uint(9, (nm + c) / 2);
        }
        sum += h;
        count++;
    }

    /* 180‑degree rotation is distinct only when both
       dimensions exceed one (otherwise it coincides with
       a reflection or identity). */
    if (r > 1 && c > 1) {
        seq_t rot;
        if ((r % 2 == 0) && (c % 2 == 0)) {
            rot = pow_uint(9, nm / 2);
        } else if ((r % 2 == 1) && (c % 2 == 1)) {
            rot = pow_uint(9, (nm + 1) / 2);
        } else {
            rot = pow_uint(9, nm / 2);
        }
        sum += rot;
        count++;
    }

    return sum / (seq_t)count;
}
