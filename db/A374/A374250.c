/*
 * A374250  Greatest sum of run-compression of a permutation of the prime
 *          factors of n.
 * https://oeis.org/A374250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Greatest sum of run‑compression of a permutation of the
 *            prime factors of n
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

typedef struct {
    uint64_t p;   /* prime value */
    uint32_t c;   /* exponent (multiplicity) */
} prime_exp_t;

/* Comparison for qsort: descending by prime value */
static int cmp_desc(const void *a, const void *b)
{
    const prime_exp_t *pa = (const prime_exp_t *)a;
    const prime_exp_t *pb = (const prime_exp_t *)b;
    if (pa->p < pb->p) return 1;
    if (pa->p > pb->p) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Treat n = 1 as special: no prime factors */
    if (n == 1)
        return (seq_t)0;

    /* Factor n.  We use small static buffers; n < 2^64 has at most 12 distinct primes. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Convert to array of structs for sorting later */
    prime_exp_t pe[64];
    size_t i;
    size_t total_factors = 0;    /* sum of multiplicities N */
    for (i = 0; i < cnt; ++i) {
        pe[i].p = primes[i];
        pe[i].c = (uint32_t)exps[i];
        total_factors += exps[i];
    }

    /* Find maximum total selected S such that
       sum_i min(c_i, floor((S+1)/2)) >= S
       Iterate S from total_factors down to 1. */
    size_t Smax = 0;
    for (size_t S = total_factors; S > 0; --S) {
        size_t M = (S + 1) / 2;   /* floor((S+1)/2) */
        size_t avail = 0;
        for (i = 0; i < cnt; ++i) {
            avail += (pe[i].c <= M) ? pe[i].c : M;
            if (avail >= S) /* early exit if already enough */
                break;
        }
        if (avail >= S) {
            Smax = S;
            break;
        }
    }
    /* Handle the impossible case (should not occur for n > 1) */
    if (Smax == 0)
        return (seq_t)0;

    /* Now we know Smax and M = floor((Smax+1)/2).  Sort primes descending. */
    size_t M = (Smax + 1) / 2;
    qsort(pe, cnt, sizeof(prime_exp_t), cmp_desc);

    /* Pick copies greedily from largest primes to reach Smax copies. */
    size_t remaining = Smax;
    seq_t total = 0;
    for (i = 0; i < cnt && remaining > 0; ++i) {
        size_t allowed = (pe[i].c <= M) ? pe[i].c : M;
        size_t take = (allowed <= remaining) ? allowed : remaining;
        total += (seq_t)pe[i].p * (seq_t)take;
        remaining -= take;
    }

    return total;
}
