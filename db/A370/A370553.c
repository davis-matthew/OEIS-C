/*
 * A370553  a(n) is the numerator of the imaginary part of Product_{k=1..n} (1
 *          + i/k) where i is the imaginary unit.
 * https://oeis.org/A370553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   a(n) = numerator of the imaginary part of ∏_{k=1}^n (1 + i/k)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 16, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for seq_t; works for signed types and arbitrary‐precision integers. */
static seq_t seq_gcd(seq_t a, seq_t b) {
    if (a < (seq_t)0) a = -a;
    if (b < (seq_t)0) b = -b;
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;            /* defined only for n ≥ 1 */

    seq_t Ar = (seq_t)1;       /* real part numerator   */
    seq_t Ai = (seq_t)0;       /* imaginary part numerator */
    seq_t Den = (seq_t)1;      /* common denominator    */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t step = (seq_t)k;

        seq_t Ar_new = Ar * step - Ai;
        seq_t Ai_new = Ar + Ai * step;
        seq_t Den_new = Den * step;

        /* Reduce the triple (Ar_new, Ai_new, Den_new) */
        seq_t g = seq_gcd(Ar_new, Ai_new);
        g = seq_gcd(g, Den_new);
        if (g > (seq_t)1) {
            Ar_new /= g;
            Ai_new /= g;
            Den_new /= g;
        }

        Ar = Ar_new;
        Ai = Ai_new;
        Den = Den_new;
    }

    /* Final reduction to ensure Ai and Den are coprime */
    seq_t g_final = seq_gcd(Ai, Den);
    if (g_final > (seq_t)1) Ai /= g_final;

    return Ai;
}
