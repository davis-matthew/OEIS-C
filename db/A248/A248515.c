/*
 * A248515  Least number k such that 1 - k*sin(1/k) < 1/n^2.
 * https://oeis.org/A248515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n / sqrt(6))
 * Range:     u8 n <= 15, u16 n <= 259, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only.
 * a(n) is the smallest integer k such that 6*k^2 >= n^2.
 * This is equivalent to ceil(n / sqrt(6)).

 * We compute:
 *   t = ceil(n^2 / 6) = (n^2 + 5) / 6
 *   root = floor_sqrt(t)
 *   if root^2 < t, increment root
 *   return root
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is idx_t (uint64_t), n >= 1 */
    seq_t n2   = (seq_t)n * (seq_t)n;     /* n^2  */
    seq_t t    = (n2 + 5) / 6;            /* ceil(n^2/6) */
    seq_t root = floor_sqrt(t);           /* floor(sqrt(t)) */

    if (root * root < t)                  /* adjust if not perfect square */
        root += 1;

    return root;
}
