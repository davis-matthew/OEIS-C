/*
 * A392519  Numbers k such that sigma(k) = psi(k) + tau(k)^4 + omega(k).
 * https://oeis.org/A392519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 1297 * p_n^2 where p_n is the n-th prime * (ignoring the
 *            prime 1297 itself). The sequence is therefore * 1297·p^2 for
 *            every prime p ≠ 1297. The first term (n=1) is * 1297·2^2 = 5188.
 *            For n=0 the function returns 0 (offset is 1)
 * Range:     u8 none, u16 n <= 4, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, so n≥1 in normal use */

    idx_t i = 1;          /* 1‑based index into the prime list */
    idx_t count = 0;      /* number of primes accepted so far */
    uint64_t p = 0;       /* prime value */

    while (count < n) {
        p = get_prime(i);   /* fetch i‑th prime (unsigned 64‑bit) */
        i++;
        if (p == 1297)       /* skip the forbidden prime */
            continue;
        count++;
    }

    seq_t ps = (seq_t)p;
    seq_t sq = ps * ps;
    return (seq_t)1297 * sq;
}
