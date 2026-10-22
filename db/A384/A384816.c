/*
 * A384816  Sum of the cubes of the indices of distinct prime factors of n.
 * https://oeis.org/A384816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} pi(p)^3 where pi(p) is the number of
 *            primes <= p
 * Range:     u8 n <= 16, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Factor n: obtain distinct prime factors.
       We allocate space for at most 64 distinct primes, which
       is more than enough even for 64‑bit integers. */
    uint64_t primes[64];
    uint64_t exps[64];   /* exps unused but required by the API */
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = primes[i];
        seq_t pi = compute_pi(p);          /* pi(p) */
        seq_t pi2 = pi * pi;               /* pi(p)^2 */
        seq_t cube = pi2 * pi;              /* pi(p)^3 */
        sum += cube;
    }
    return sum;
}
