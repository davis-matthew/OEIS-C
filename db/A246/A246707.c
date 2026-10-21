/*
 * A246707  Expansion of phi(-q) * phi(-q^2) * phi(-q^3) * phi(-q^6) in powers
 *          of q.
 * https://oeis.org/A246707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246707 : coefficients of product φ(-q)·φ(-q²)·φ(-q³)·φ(-q⁶)
 *   (equal to eta(q)²·eta(q²)·eta(q³)²·eta(q⁶)/(eta(q⁴)·eta(q¹²))).
 *   The generating function is ∏_{m≥1} (1 - q^m)^{E(m)} with
 *   E(m) = 2 + 1_{2|m} + 2·1_{3|m} + 1_{6|m} – 1_{4|m} – 1_{12|m}.
 *   The table e[m mod 12] is {4,2,3,4,2,2,6,2,2,4,3,2}.
 *   Algorithm: build coefficients up to n by successive multiplication
 *   with (1 - q^m)^{e[m mod 12]} (e≤6) using in‑place descending loops.
 */

#include <stdlib.h>
#include <stdint.h>

static const int e_table[12] = {4,2,3,4,2,2,6,2,2,4,3,2};

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate coefficient array f[0..n] */
    seq_t *f = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!f)
        return (seq_t)0;  /* allocation failure: undefined behaviour for this task */

    for (idx_t i = (idx_t)0; i <= n; ++i)
        f[i] = (seq_t)0;
    f[(idx_t)0] = (seq_t)1;

    for (idx_t m = (idx_t)1; m <= n; ++m) {
        int e = e_table[m % (idx_t)12];
        for (int r = 0; r < e; ++r) {
            /* multiply by (1 - q^m) : use descending loop to avoid aliasing */
            for (idx_t j = n; j >= m; --j) {
                f[j] -= f[j - m];
            }
        }
    }

    seq_t result = f[n];
    free(f);
    return result;
}
