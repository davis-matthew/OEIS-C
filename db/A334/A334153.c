/*
 * A334153  Number of Goldbach partitions (p,q) of 2n, such that exactly one
 *          of p-2 or q-2 is prime.
 * https://oeis.org/A334153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered Goldbach partitions (p,q) of 2n for
 *            which exactly one of p-2 or q-2 is prime. Computed by
 *            brute‑force over primes p ≤ n
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime(p))
            continue;
        idx_t q = 2*n - p;
        if (!is_prime(q))
            continue;
        bool p2prime = is_prime(p-2);
        bool q2prime = is_prime(q-2);
        if (p2prime != q2prime)
            ++count;
    }
    return count;
}
