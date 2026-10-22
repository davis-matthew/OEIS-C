/*
 * A332252  a(n) is the imaginary part of f(n) defined by f(0) = 0 and f(n+1)
 *          = f(n) + i^A000120(n) (where i denotes the imaginary unit).
 *          Sequence A332251 gives real parts.
 * https://oeis.org/A332252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ_{k=0}^{n-1} δ(k), δ(k) = 1 if popcount(k)%4==1, δ(k) =
 *            -1 if popcount(k)%4==3, else 0
 * Range:     i8 n <= 8192, i16 n <= 8192, i32 n <= 8192, i64 n <= 8192,
 *            i128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k)
    {
        unsigned long long kk = (unsigned long long)k;
        unsigned pc = __builtin_popcountll(kk);
        unsigned r = pc & 3U;          /* r = pc mod 4 */
        if (r == 1U)
            sum += 1;
        else if (r == 3U)
            sum -= 1;
    }
    return sum;
}
