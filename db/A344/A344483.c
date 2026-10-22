/*
 * A344483  a(n) = n^2 + sigma(n) - n*d(n).
 * https://oeis.org/A344483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + sigma(n) - n*d(n)
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type before arithmetic */
    seq_t N = (seq_t)n;

    /* Compute n^2 */
    seq_t sq = N * N;

    /* Sum of divisors */
    seq_t sig = sigma_of(n);

    /* Number of divisors (d(n)) */
    seq_t d = (seq_t)divisor_count_u64((uint64_t)n);

    /* Result: n^2 + sigma(n) - n*d(n) */
    return sq + sig - N * d;
}
