/*
 * A365126  Number of representations of n as the sum of a prime number and a
 *          fourth power of a nonnegative integer.
 * https://oeis.org/A365126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (p, j⁴) with p prime, j ≥ 0, such that p
 *            + j⁴ = n * (OEIS sequence A365126). The calculation is performed
 *            by brute‑force search: * - For each j with j⁴ ≤ n compute r = n
 *            – j⁴. * - Count r if it is a prime number. * The function relies
 *            on the utility `bool is_prime(idx_t x)`...
 * Range:     u8 n <= 82, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t); */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, but harmless */

    seq_t count = 0;
    for (idx_t j = 0;; ++j) {
        /* Compute j⁴ safely in seq_t */
        seq_t j_seq = (seq_t)j;
        seq_t j4 = j_seq * j_seq * j_seq * j_seq;
        if (j4 > (seq_t)n)
            break;

        idx_t rem = (idx_t)((seq_t)n - j4);
        if (rem >= 2 && is_prime(rem))
            ++count;
    }
    return count;
}
