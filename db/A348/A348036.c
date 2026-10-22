/*
 * A348036  a(n) = gcd(n, A003968(n)), where A003968 is multiplicative with
 *          a(p^e) = p*(p+1)^(e-1).
 * https://oeis.org/A348036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A003968(n)), where A003968(p^e) = p * (p+1)^(e-1)
 *            and is multiplicative
 * Range:     u8 n <= 79, u16 n <= 9956, u32 n <= 9956, u64 n <= 9956,
 *            u128 n <= 9956, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static seq_t compute_A003968_uint64(uint64_t n)
{
    if (n == 1ULL)
        return (seq_t)1;

    /* Maximum distinct prime factors for 64‑bit integer is < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t product = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        seq_t  pseq = (seq_t)p;
        seq_t plus1 = (seq_t)(p + 1);

        product *= pseq;               /* multiply by p once */
        for (uint64_t j = 1; j < e; ++j)
            product *= plus1;          /* multiply by (p+1)^(e-1) */
    }
    return product;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n=0, return 0 */

    seq_t nseq = (seq_t)n;
    seq_t afunc = compute_A003968_uint64((uint64_t)n);
    return gcd(nseq, afunc);
}
