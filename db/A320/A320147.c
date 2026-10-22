/*
 * A320147  Number of integer solutions to a^2 + b^2 + 3*c^2 + 5*d^2 = n.
 * https://oeis.org/A320147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (a,b,c,d) ∈ ℤ⁴ | a² + b² + 3c² + 5d² = n }
 * Range:     u8 n <= 56, u16 n <= 9456, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_two_square_reps
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_two_square_reps.h"

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;
    for (idx_t d = 0; 5ULL * d * d <= n; ++d) {
        idx_t rem1 = n - 5ULL * d * d;
        seq_t weight_d = (d == 0) ? (seq_t)1 : (seq_t)2;

        for (idx_t c = 0; 3ULL * c * c <= rem1; ++c) {
            idx_t rem = rem1 - 3ULL * c * c;
            seq_t weight_c = (c == 0) ? (seq_t)1 : (seq_t)2;

            seq_t reps = count_two_square_reps(rem);
            total += (weight_d * weight_c) * reps;
        }
    }
    return total;
}
