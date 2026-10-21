/*
 * A298756  Least strong pseudoprime to base n.
 * https://oeis.org/A298756
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least odd composite m such that m is a strong pseudoprime
 *            to base n * (i.e., n^d ≡ 1 (mod m) or n^(2^r d) ≡ -1 (mod m) for
 *            some 0 ≤ r < s, * where m-1 = 2^s · d, d odd). The function
 *            searches only odd m ≥ 3 and * skips primes. For n ≥ 2 it returns
 *            the required pseudoprime
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* undefined for n < 2 */
    if (n < 2)
        return 0;

    /* Search over odd composites only */
    for (idx_t m = 3; ; m += 2)          /* m is odd ≥ 3 */
    {
        /* Skip primes */
        if (is_prime_big((seq_t)m))
            continue;

        /* Factor m-1 = 2^s · d, d odd */
        idx_t d = m - 1;
        idx_t s = 0;
        while ((d & 1) == 0) {
            d >>= 1;
            ++s;
        }

        /* Reduce base modulo m to keep numbers small */
        seq_t base = (seq_t)n;
        if (base >= (seq_t)m)
            base = base % (seq_t)m;

        /* Compute n^d (mod m) */
        seq_t x = mod_pow(base, d, (seq_t)m);

        /* Strong pseudoprime test */
        if (x == (seq_t)1 || x == ((seq_t)m - (seq_t)1))
            return (seq_t)m;

        for (idx_t r = 1; r < s; ++r) {
            /* Square x modulo m */
            x = mod_pow(x, 2, (seq_t)m);
            if (x == ((seq_t)m - (seq_t)1))
                return (seq_t)m;
        }
        /* Otherwise continue searching */
    }
}
