/*
 * A258358  Sum over all partitions lambda of n into 3 distinct parts of
 *          Product_{i:lambda} prime(i).
 * https://oeis.org/A258358
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all triples 1≤a<b<c with a+b+c=n of
 *            prime(a)*prime(b)*prime(c) where prime(k) is the k‑th prime
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 134, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 6) return 0;  /* values below the offset are undefined */

    seq_t sum = 0;

    for (idx_t a = 1; a <= n - 3; ++a) {
        /* b must satisfy a < b < c = n - a - b,
           so b < (n - a)/2  */
        idx_t max_b = (n - a - 1) / 2;
        for (idx_t b = a + 1; b <= max_b; ++b) {
            idx_t c = n - a - b;
            if (c <= b) continue; /* safety, should never happen */
            seq_t prod = (seq_t)get_prime(a) * (seq_t)get_prime(b) * (seq_t)get_prime(c);
            sum += prod;
        }
    }

    return sum;
}
