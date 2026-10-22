/*
 * A325033  Sum of sums of the multisets of prime indices of each prime index
 *          of n.
 * https://oeis.org/A325033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = SUM over prime factors p^e of n of e * SUM over prime *
 *            factors q^f of index(p) of f * index(q)
 * Range:     u8 n <= 4176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper functions factor_u64() and prime_pi() */

#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    seq_t result = 0;

    /* factor the input n */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    for (size_t i = 0; i < pcnt; i++) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* index of prime p */
        idx_t ip = (idx_t)prime_pi(p);
        if (ip == 0) continue;          /* should not happen for p >= 2 */

        /* factor the index ip */
        uint64_t ipr[64], ipex[64];
        size_t ipcnt = 0;
        factor_u64((uint64_t)ip, ipr, ipex, &ipcnt);

        seq_t sum_ip = 0;
        for (size_t j = 0; j < ipcnt; j++) {
            uint64_t q = ipr[j];
            uint64_t f = ipex[j];

            /* index of prime q */
            idx_t iq = (idx_t)prime_pi(q);
            if (iq > 0)
                sum_ip += (seq_t)f * (seq_t)iq;
        }

        result += (seq_t)e * sum_ip;
    }

    return result;
}
