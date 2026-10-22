/*
 * A339115  Greatest semiprime whose prime indices sum to n.
 * https://oeis.org/A339115
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1 ≤ i < n} ( prime_i * prime_{n-i} ) where prime_k
 *            is the k‑th prime number. The definition follows the OEIS
 *            specification: for each index n ≥ 2, consider all pairs of prime
 *            indices (i, j) with i + j = n and take the greatest semiprime
 *            produced by their product
 * Range:     u8 n <= 13, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                         /* undefined for indices < 2 */

    seq_t maxp = 0;
    for (idx_t i = 1; i < n; ++i) {
        idx_t j = n - i;
        seq_t left  = (seq_t)get_prime(i);
        seq_t right = (seq_t)get_prime(j);
        seq_t prod  = left * right;
        if (prod > maxp)
            maxp = prod;
    }
    return maxp;
}
