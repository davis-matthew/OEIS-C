/*
 * A363944  Mean of the multiset of prime indices of n, rounded up.
 * https://oeis.org/A363944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( (∑_{p^e‖n} e·π(p)) / (∑_{p^e‖n} e) ), where π(p) is
 *            the prime index of p (i.e., the number of primes ≤ p)
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;            /* a(1) = 0 by definition */

    /* prime factorization of n: primes[i]^exps[i] (i < cnt) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;   /* Σ e·π(p) */
    seq_t total = 0; /* Σ e */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t idx = compute_pi(primes[i]);          /* π(p) */
        seq_t exp = (seq_t)exps[i];                 /* e */
        sum   += idx * exp;
        total += exp;
    }

    if (total == 0)
        return 0;   /* safety for n=1, but handled earlier */

    /* ceil division: (sum + total - 1) / total */
    return (sum + total - 1) / total;
}
