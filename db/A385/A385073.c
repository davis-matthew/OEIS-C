/*
 * A385073  a(n) = b^(n-1) mod n, where b = A053669(n) is the least integer
 *          greater than 1 and coprime to n.
 * https://oeis.org/A385073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = b^(n-1) mod n, where b is the least integer >1 coprime to
 *            n
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t sn = (seq_t)n;          /* n as seq_t for gcd and modulo operations */
    seq_t b = (seq_t)2;           /* start searching for the least coprime integer */

    /* Find the smallest integer >1 that is coprime to n */
    while (gcd(b, sn) != (seq_t)1)
    {
        b++;
    }

    /* Compute b^(n-1) mod n */
    return mod_pow(b, (uint64_t)(n - 1), sn);
}
