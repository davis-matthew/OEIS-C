/*
 * A333341  a(n) is the smallest positive number k such that n divides 5^k -
 *          k.
 * https://oeis.org/A333341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive k such that n | 5^k - k
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute A333341 for index n (n >= 1). */
seq_t a(idx_t n)
{
    /* N is small (≤ 10 000), so we can safely use uint64_t for
       intermediate modular arithmetic. */
    uint64_t mod = (uint64_t)n;     /* modulus */

    /* 5^k modulo n, starting with k = 1 */
    uint64_t powk = 5 % mod;

    /* Search for the smallest k.  Because mod is a positive integer,
       this loop is guaranteed to terminate. */
    for (uint64_t k = 1;; ++k) {
        uint64_t r = k % mod;          /* k modulo n */
        if (powk == r) {
            /* Return k as a seq_t value.  Casting is safe because k
               fits in uint64_t, which is ≤ the maximum value that
               seq_t can represent for the given build. */
            return (seq_t)k;
        }
        /* Prepare 5^(k+1) mod n for the next iteration. */
        powk = (powk * 5) % mod;
    }
}
