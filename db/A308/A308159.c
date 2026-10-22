/*
 * A308159  Sum of the largest sides of all integer-sided isosceles triangles
 *          with perimeter n.
 * https://oeis.org/A308159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=a_min}^{a_max} max(a, n-2a) where a_min =
 *            floor(n/4)+1, a_max = floor((n-1)/2)
 * Range:     u8 n <= 50, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* No isosceles triangles for n < 3 */
    if (n < 3) return 0;

    /* Compute the admissible range of the equal side a */
    idx_t a_min = n / 4 + 1;
    idx_t a_max = (n - 1) / 2;
    if (a_min > a_max) return 0;

    /* Helper: cast for arithmetic in seq_t registers */
    seq_t n_seq = (seq_t)n;

    /* Compute the break point a = floor((n-1)/3) where 3*a < n */
    idx_t fl_n3 = (n - 1) / 3;

    /* --- Small a: 3*a < n, largest side is b = n-2a --- */
    idx_t a_small_start = a_min;
    idx_t a_small_end   = a_max < fl_n3 ? a_max : fl_n3;

    seq_t sum_small = 0;
    if (a_small_start <= a_small_end) {
        seq_t count = (seq_t)(a_small_end - a_small_start + 1);

        /* Sum of a from a_small_start to a_small_end */
        seq_t sum_a = ((seq_t)a_small_end * (a_small_end + 1) / 2) -
                      ((seq_t)(a_small_start - 1) * a_small_start / 2);

        sum_small = count * n_seq - ((seq_t)2 * sum_a);
    }

    /* --- Large a: 3*a >= n, largest side is a --- */
    idx_t big_start = a_min > fl_n3 + 1 ? a_min : fl_n3 + 1; /* first a with 3*a >= n */
    seq_t sum_large = 0;
    if (big_start <= a_max) {
        /* Sum of a from big_start to a_max */
        seq_t sum_a_large = ((seq_t)a_max * (a_max + 1) / 2) -
                            ((seq_t)(big_start - 1) * big_start / 2);
        sum_large = sum_a_large;
    }

    return sum_small + sum_large;
}
