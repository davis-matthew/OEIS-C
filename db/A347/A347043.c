/*
 * A347043  Smallest divisor of n with half (rounded up) as many prime factors
 *          (counting multiplicity) as n.
 * https://oeis.org/A347043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor of n with half (rounded up) as many
 *            prime factors (counted with multiplicity) as n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses the provided helpers:
 *    - bigomega          : total number of prime factors of n (counting multiplicity)
 *    - factor_u64        : prime factorisation of an unsigned 64‑bit integer
 *    - seq_pow           : exponentiation in the seq_t type
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/bigomega.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

/* Recursive helper to explore all divisors of n */
static void dfs(const uint64_t *primes, const uint64_t *exps, size_t idx, size_t cnt,
                seq_t current, int currOmega, idx_t target, seq_t *best)
{
    if (idx == cnt) {
        if (currOmega == target) {
            if ((*best == 0) || (current < *best))
                *best = current;
        }
        return;
    }

    /* Enumerate exponent e for the current prime */
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        seq_t next = current;
        if (e > 0) {
            seq_t pp = seq_pow((seq_t)primes[idx], (idx_t)e);
            next = current * pp;
        }
        dfs(primes, exps, idx + 1, cnt, next, currOmega + (int)e, target, best);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n == 1 is a boundary case */
    if (n == 1) return (seq_t)1;

    /* Compute target number of prime factors */
    seq_t om = bigomega(n);
    idx_t omega = (idx_t)om;
    idx_t target = (omega + 1) / 2;      /* ceil(omega/2) */

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Enumerate divisors and keep the smallest with the required Omega */
    seq_t best = 0;   /* 0 will serve as a sentinel for “not yet set” */
    dfs(primes, exps, 0, cnt, (seq_t)1, 0, target, &best);

    return best;
}
