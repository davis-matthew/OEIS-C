/*
 * A396923  a(n) is the number of grand Motzkin paths with air pockets of
 *          length n.
 * https://oeis.org/A396923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{d=1..floor(n/2)} binomial(n - d + 1, d) *
 *            Sum_{i=0..n-2*d} binomial(n - d, i) * binomial(n - d - i - 1, d
 *            - 1) a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 1;                    // the term a(0)
    idx_t max_d = n >> 1;                // floor(n/2)

    for (idx_t d = 1; d <= max_d; ++d) {
        idx_t N = n - d;                 // N = n - d
        idx_t m = N - d;                 // upper limit for i = n - 2*d

        // outer binomial factor
        seq_t outer = binomial(n - d + 1, d);

        // inner sum
        seq_t inner = 0;
        seq_t A = 1;                     // binomial(N, 0)
        seq_t B = binomial(N - 1, d - 1); // binomial(N-1, d-1) for i=0
        idx_t k = N - 1;                 // current first argument of B

        for (idx_t i = 0; i <= m; ++i) {
            inner += A * B;

            if (i == m) break;          // last term reached

            // Update A = C(N, i+1)
            A = A * (seq_t)(N - i) / (seq_t)(i + 1);

            // Update B = C(k-1, d-1) where k = N - i - 1
            //   C(k-1, r) = C(k, r) * (k - r) / k
            B = B * (seq_t)(k - (d - 1)) / (seq_t)k;
            --k;                         // advance k for next iteration
        }

        result += outer * inner;
    }

    return result;
}
