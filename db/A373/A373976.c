/*
 * A373976  a(n) = A001222(n) - A001222(A001414(n)), where A001222 is
 *          bigomega, the number of prime factors with multiplicity, and
 *          A001414 is sopfr, sum of prime factors with multiplicity. a(1) = 0
 *          by convention.
 * https://oeis.org/A373976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = bigomega(n) - bigomega(sopfr(n))
 * Range:     i8 n <= 9852, i16 n <= 9852, i32 n <= 9852, i64 n <= 9852,
 *            i128 n <= 9852, big any n
 * Helpers:   bigomega, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)                     /* a(1) = 0 by convention */
        return 0;
    seq_t om_n = bigomega(n);       /* bigomega(n) */

    /* Factor n to compute sopfr(n) = sum of prime factors with multiplicity */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t sop = 0;
    for (size_t i = 0; i < cnt; ++i)
        sop += primes[i] * exps[i]; /* sopfr(n) */

    seq_t om_sop = bigomega((idx_t)sop); /* bigomega(sopfr(n)) */

    return om_n - om_sop;
}
