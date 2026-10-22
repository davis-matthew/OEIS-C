/*
 * A373588  a(n) = 1 if the number of prime factors (with multiplicity) of n
 *          is a multiple of 3, and all of them are of the type 3m-1 (in
 *          A003627), otherwise 0.
 * https://oeis.org/A373588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n has a prime factorization in which every prime
 *            factor p satisfies p ≡ 2 (mod 3) and the total number of prime
 *            factors (counting multiplicities) is a multiple of 3; otherwise
 *            0
 * Range:     u8 n <= 9135, u16 n <= 9135, u32 n <= 9135, u64 n <= 9135,
 *            u128 n <= 9135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    idx_t m = n;
    idx_t count = 0;
    bool bad = false;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            while (m % p == 0) {
                ++count;
                if (p % 3 != 2) {
                    bad = true;
                }
                m /= p;
            }
        }
    }
    if (m > 1) {                   /* remaining prime factor */
        ++count;
        if (m % 3 != 2) bad = true;
    }
    if (bad) return 0;
    return (seq_t)((count % 3 == 0) ? 1 : 0);
}
