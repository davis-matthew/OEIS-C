/*
 * A307100  a(n) = Sum_{d|n} (tau(d)*pod(d)) where tau(k) = the number of
 *          divisors of k (A000005) and pod(k) = the product of the divisors
 *          of k (A007955).
 * https://oeis.org/A307100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} tau(d)*pod(d)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   factor_u64, ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(k) = number of divisors of k (A000005)            */
/* pod(k) = product of the divisors of k (A007955)        */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Recursive helper to accumulate the sum over all divisors. */
static void dfs(size_t idx,
                seq_t cur_div,
                seq_t cur_tau,
                const uint64_t *primes,
                const uint64_t *exps,
                size_t cnt,
                seq_t *sum)
{
    if (idx == cnt) {
        /* compute pod(cur_div) = cur_div^(cur_tau/2)  (with an extra sqrt if cur_tau is odd) */
        seq_t half = cur_tau / 2;                     /* integer division */
        seq_t pod  = ipow(cur_div, half);
        if ((seq_t)(cur_tau & 1))                    /* if cur_tau is odd */
            pod = pod * floor_sqrt(cur_div);
        *sum += cur_tau * pod;
        return;
    }

    uint64_t p  = primes[idx];
    size_t   e  = (size_t)exps[idx];
    seq_t    pk = 1;                                 /* p^k */

    for (size_t k = 0; k <= e; ++k) {
        seq_t new_div = cur_div * pk;
        seq_t new_tau = cur_tau * (seq_t)(k + 1);
        dfs(idx + 1, new_div, new_tau, primes, exps, cnt, sum);
        pk *= (seq_t)p;                              /* next power of p */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs(0, 1, 1, primes, exps, cnt, &sum);
    return sum;
}
