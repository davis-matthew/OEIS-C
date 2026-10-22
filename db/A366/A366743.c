/*
 * A366743  The sum of infinitary divisors of the least coreful infinitary
 *          divisor of n.
 * https://oeis.org/A366743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Product over p^e||n of (p^{v2(e)} + 1) where v2(e) is the
 *            largest power of 2 that divides e (i.e. e & -e ). This follows
 *            from a(p^e) = p^{A006519(e)} + 1 with A006519(e) = v2(e) for all
 *            e≥1
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 1)          /* a(1) = 1 */
        return (seq_t)1;

    seq_t result = 1;
    idx_t temp  = n;

    /* handle prime 2 */
    idx_t e = 0;
    while ((temp & 1ULL) == 0ULL) { /* temp % 2 == 0 */
        temp >>= 1;
        ++e;
    }
    if (e) {
        idx_t v = e & -e;          /* largest power of two dividing e */
        seq_t term = pow_seq((seq_t)2, (int)v) + (seq_t)1;
        result *= term;
    }

    /* odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p != 0)
            continue;
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        idx_t v = exp & -exp;
        seq_t term = pow_seq((seq_t)p, (int)v) + (seq_t)1;
        result *= term;
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        idx_t exp = 1;
        idx_t v = exp & -exp;      /* v == 1 */
        seq_t term = pow_seq((seq_t)temp, (int)v) + (seq_t)1;
        result *= term;
    }

    return result;
}
