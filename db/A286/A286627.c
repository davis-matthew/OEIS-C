/*
 * A286627  a(n) = exponent of the highest power of A000005(n) (number of
 *          divisors of n) dividing A000010(n) (totient function phi), a(1) =
 *          1.
 * https://oeis.org/A286627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of the highest power of d(n) that divides φ(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(n) = number of divisors of n and φ(n) is Euler's totient. */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    uint64_t nn = (uint64_t)n;          /* use 64‑bit arithmetic for n */
    /* compute d(n) */
    seq_t d_seq = (seq_t)divisor_count_u64(nn);
    uint64_t d = (uint64_t)d_seq;       /* d fits into 64 bits for n ≤ 10000 */

    /* factor n: primes[], exps[] and cnt */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* compute φ(n) */
    uint64_t phi = nn;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        phi = phi / p * (p - 1);
    }

    /* find largest e with d^e | φ(n) */
    seq_t e = 0;
    while (phi % d == 0) {
        phi /= d;
        e++;
    }
    return e;
}
