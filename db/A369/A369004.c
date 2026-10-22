/*
 * A369004  a(n) = 1 if n' / gcd(n,n') is a multiple of 4, otherwise 0, where
 *          n' stands for the arithmetic derivative of n, A003415(n).
 * https://oeis.org/A369004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if (n' / gcd(n,n')) % 4 == 0 else 0, where n' is the
 *            arithmetic derivative of n
 * Range:     u8 n <= 1723, u16 n <= 9888, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Compute n' = arithmetic derivative of n */
    seq_t n_seq = (seq_t)n;                /* cast index to seq_t for arithmetic */
    uint64_t pr[64];                       /* primes of n */
    uint64_t ex[64];                       /* corresponding exponents */
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt); /* factor n */

    seq_t der = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t term = (n_seq / (seq_t)pr[i]) * (seq_t)ex[i];
        der += term;
    }

    /* Compute n' / gcd(n,n') and test divisibility by 4 */
    seq_t g = gcd(der, n_seq);
    seq_t q = der / g;
    return (q % (seq_t)4 == (seq_t)0) ? (seq_t)1 : (seq_t)0;
}
