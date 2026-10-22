/*
 * A378301  a(n) is the number of triangular numbers (A000217) in the interval
 *          [n^2, (n + 1)^2].
 * https://oeis.org/A378301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triangular numbers t_k = k(k+1)/2 in * the
 *            interval [n^2, (n+1)^2] for n ≥ 0. The implementation *
 *            evaluates the bounds via integer arithmetic and floor_sqrt
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t triangular(seq_t k)
{
    return k * (k + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Lower bound n^2 */
    seq_t n_sq       = (seq_t)n * (seq_t)n;
    seq_t inner_low  = 8 * n_sq + 1;
    seq_t sqrt_low   = floor_sqrt(inner_low);
    seq_t k_min      = sqrt_low / 2;          /* floor((√D - 1)/2) */
    /* Adjust upward until t_k ≥ n^2 */
    while (triangular(k_min) < n_sq)
        ++k_min;

    /* Upper bound (n+1)^2 */
    idx_t n1         = n + 1;
    seq_t n1_sq      = (seq_t)n1 * (seq_t)n1;
    seq_t inner_up   = 8 * n1_sq + 1;
    seq_t sqrt_up    = floor_sqrt(inner_up);
    seq_t k_max      = (sqrt_up - 1) / 2;     /* floor((√D - 1)/2) */
    /* Adjust upward until t_k > (n+1)^2 */
    while (triangular(k_max + 1) <= n1_sq)
        ++k_max;

    if (k_max < k_min)
        return 0;
    return k_max - k_min + 1;
}
