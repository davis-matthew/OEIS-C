/*
 * A305611  Number of distinct positive subset-sums of the multiset of prime
 *          factors of n.
 * https://oeis.org/A305611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of distinct positive subset-sums of the multiset of *
 *            prime factors of n. The value is the count of distinct sums that
 *            can * be obtained by summing a non‑empty submultiset of the
 *            prime factor * multiset of n. For n=1 the result is 0
 * Range:     u8 n <= 9374, u16 n <= 9374, u32 n <= 9374, u64 n <= 9374,
 *            u128 n <= 9374, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    if (n < 1u)            /* offset is 1, behaviour for n<1 is irrelevant */
        return (seq_t)0;

    if (n == 1u)           /* 1 has no prime factors */
        return (seq_t)0;

    /* Prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Maximum possible subset‑sum = sum of all prime factors counted with multiplicity */
    size_t max_sum = 0;
    for (size_t i = 0; i < pcnt; ++i)
        max_sum += (size_t)primes[i] * (size_t)exps[i];

    /* dp[s] == true  ⇔  sum s is attainable with a non‑empty submultiset
     * (we initialise dp[0] = true to allow initial additions). */
    bool *dp = (bool *)calloc(max_sum + 1u, sizeof(bool));
    if (!dp)          /* allocation failure → safety return 0 */
        return (seq_t)0;
    dp[0] = true;

    /* For each prime factor (with its multiplicity) add its contribution
     * to the subset sums.  We handle multiplicity by iterating the
     * prime the required number of times. */
    for (size_t i = 0u; i < pcnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        for (uint64_t k = 0u; k < exp; ++k) {
            for (size_t sum = max_sum; sum >= (size_t)p; --sum) {
                if (dp[sum - (size_t)p])
                    dp[sum] = true;
            }
        }
    }

    /* Count distinct positive subset-sums */
    seq_t count = 0;
    for (size_t sum = 1u; sum <= max_sum; ++sum)
        if (dp[sum])
            ++count;

    free(dp);
    return count;
}
