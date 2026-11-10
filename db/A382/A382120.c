/*
 * A382120  Numbers k in A024619 such that there exists a prime p | k for
 *          which p^(m+1) == r (mod k), where r is also in A024619, and a
 *          prime q | k for which q^(m+1) == r (mod k), where r is a prime
 *          power.
 * https://oeis.org/A382120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k in A024619 (not a prime power) such that, with r_p =
 *            p^(m+1) mod k where p^m <= k < p^(m+1) (smallest power of p
 *            exceeding k), some prime p | k has r_p in A024619 and some prime
 *            q | k has r_q a prime power
 * Range:     u8 n <= 92, u16 n <= 9991, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9870 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether x is a power of a prime (including 1 = p^0). */
static bool A382120_is_prime_power(uint64_t x)
{
    if (x < 2)
        return true;
    uint64_t rem = x;
    int distinct = 0;
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            if (++distinct > 1)
                return false;
            while (rem % p == 0)
                rem /= p;
        }
    }
    if (rem > 1)
        ++distinct;
    return distinct == 1;
}

static bool A382120_test(uint64_t k)
{
    uint64_t primes[64];
    int np = 0;
    uint64_t rem = k;
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            primes[np++] = p;
            while (rem % p == 0)
                rem /= p;
        }
    }
    if (rem > 1)
        primes[np++] = rem;
    if (np < 2)
        return false;               /* k is a prime power (or 1) */

    bool has_comp = false, has_pp = false;
    for (int i = 0; i < np; ++i) {
        uint64_t p = primes[i];
        uint64_t pw = 1;
        while (pw <= k)
            pw *= p;                /* smallest power of p exceeding k */
        uint64_t r = pw % k;
        if (A382120_is_prime_power(r))
            has_pp = true;
        else
            has_comp = true;
        if (has_pp && has_comp)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static uint64_t *seq = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t next_k = 2;

    if (n == 0)
        return (seq_t)0;
    size_t target = (size_t)(n - 1);

    while (len <= target) {
        if (len == cap) {
            size_t nc = cap ? cap * 2 : 256;
            uint64_t *tmp = (uint64_t *)realloc(seq, nc * sizeof(uint64_t));
            if (!tmp)
                return (seq_t)0;
            seq = tmp;
            cap = nc;
        }
        while (!A382120_test(next_k))
            ++next_k;
        seq[len++] = next_k++;
    }
    return (seq_t)seq[target];
}
