/*
 * A336597  a(n) is the least number k of the form (the number of nonnegative
 *          bases m < n such that m^k == m (mod n))/(the number of nonnegative
 *          bases m < n such that -m^k == m (mod n)) dividing n, or 0 if no
 *          such k exists.
 * https://oeis.org/A336597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implementation of OEIS sequence A336597.
 *
 * Definition: a(n) is the least number k (k|n) such that
 *     k = (#{ m < n | m^k ≡ m (mod n) }) / (#{ m < n | -m^k ≡ m (mod n) }).
 * If no such k exists, a(n) = 0.
 *
 * The algorithm iterates over all divisors k of n, computes the two
 * counts for each k by brute force (n ≤ 10000), and tests the
 * equality condition.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Computes the number of divisors of n in ascending order. */
static void divisors(uint64_t n, uint64_t **divs, size_t *cnt)
{
    size_t capacity = 16, size = 0;
    uint64_t *tmp = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    for (uint64_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            if (size == capacity) { capacity *= 2; tmp = (uint64_t *)realloc(tmp, capacity * sizeof(uint64_t)); }
            tmp[size++] = i;
            if (i * i != n)
            {
                if (size == capacity) { capacity *= 2; tmp = (uint64_t *)realloc(tmp, capacity * sizeof(uint64_t)); }
                tmp[size++] = n / i;
            }
        }
    }
    /* sort ascending */
    for (size_t i = 0; i < size; ++i)
        for (size_t j = i + 1; j < size; ++j)
            if (tmp[i] > tmp[j])
            {
                uint64_t tmpv = tmp[i]; tmp[i] = tmp[j]; tmp[j] = tmpv;
            }
    *divs = tmp;
    *cnt  = size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* should never happen, but guard */
    uint64_t N = (uint64_t)n;          /* use 64-bit for safety */
    uint64_t *divs = NULL;
    size_t divcnt = 0;
    divisors(N, &divs, &divcnt);

    for (size_t di = 0; di < divcnt; ++di)
    {
        uint64_t k = divs[di];          /* k divides n */
        seq_t cntA = 0, cntB = 0;
        seq_t mod = (seq_t)N;
        for (uint64_t m = 0; m < N; ++m)
        {
            seq_t powm = mod_pow((seq_t)m, k, mod);
            if (powm == (seq_t)m) cntA++;
            seq_t minus = (mod - (powm % mod)) % mod;
            if (minus == (seq_t)m) cntB++;
        }
        if (cntB == 0) continue;           /* avoid division by zero */
        if (cntA % cntB != 0) continue;
        seq_t candidate = cntA / cntB;
        if (candidate == (seq_t)k)         /* k divides n by construction */
        {
            free(divs);
            return candidate;
        }
    }

    free(divs);
    return 0;   /* no suitable k */
}
