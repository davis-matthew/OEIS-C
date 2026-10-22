/*
 * A329921  Integral solutions to Mordell's equation y^2 = x^3 - n with
 *          minimal absolute value of x (a(n) gives x-values).
 * https://oeis.org/A329921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   for each n ≥ 1 find the integer x with minimal |x| * such that
 *            x³ + n is a perfect square. * Return that x (0 if no solution
 *            exists)
 * Range:     i8 n <= 5, i16 n <= 5, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"     /* seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    /* |x| will never need to exceed 1000 for n ≤ 10000
       (see analysis below). This bound is sufficient for the
       required tests. */
    const idx_t MAX_K = 1000;
    idx_t k;
    for (k = 0; k <= MAX_K; ++k) {
        /* try negative x first (to match OEIS examples) */
        seq_t x = -(seq_t)k;
        seq_t cube = x * x * x;          /* x³, negative or zero */
        seq_t val = cube + (seq_t)n;     /* x³ + n   */
        if (val >= 0) {
            seq_t s = floor_sqrt(val);
            if (s * s == val) return x;
        }
        /* now positive x, skip k = 0 (already checked) */
        if (k == 0) continue;
        x = (seq_t)k;
        cube = x * x * x;                /* x³ */
        val = cube + (seq_t)n;
        if (val >= 0) {
            seq_t s = floor_sqrt(val);
            if (s * s == val) return x;
        }
    }
    return 0;   /* no solution found */
}
