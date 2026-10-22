/*
 * A378447  Difference between {the number of distinct sums of distinct
 *          divisors of n} and {the number of distinct sums of distinct proper
 *          divisors of n}.
 * https://oeis.org/A378447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8707, u32 n <= 8707, u64 n <= 8707,
 *            u128 n <= 8707, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Difference between the number of distinct sums of distinct divisors of n
   and of distinct proper divisors of n.  This implementation computes
   the counts via subset–sum dynamic programming. */
#include <stdlib.h>
#include <stdint.h>

/* Helper to generate all prime factors of n */
static void factorize_uint64(uint64_t n, uint64_t *p, int *e, size_t *cnt)
{
    *cnt = 0;
    uint64_t temp = n;
    for (uint64_t d = 2; d <= temp / d; ++d)
    {
        if (temp % d == 0)
        {
            int ex = 0;
            while (temp % d == 0)
            {
                temp /= d;
                ++ex;
            }
            p[*cnt] = d;
            e[*cnt] = ex;
            ++(*cnt);
        }
    }
    if (temp > 1)
    {
        p[*cnt] = temp;
        e[*cnt] = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    /* Special case for n==0 (out of offset), return 0 */
    if (n == 0) return (seq_t)0;

    /* Factorisation of n */
    uint64_t primes[64];
    int exp[64];
    size_t pfcnt = 0;
    factorize_uint64((uint64_t)n, primes, exp, &pfcnt);

    /* Compute the total number of divisors (including n) */
    size_t totdiv = 1;
    for (size_t i = 0; i < pfcnt; ++i)
        totdiv *= (size_t)(exp[i] + 1);

    /* Generate all divisors */
    uint64_t *divs = (uint64_t *)calloc(totdiv, sizeof(uint64_t));
    divs[0] = 1;
    size_t curcnt = 1;
    for (size_t i = 0; i < pfcnt; ++i)
    {
        uint64_t p = primes[i];
        int e = exp[i];
        size_t oldcnt = curcnt;

        /* Compute successive powers of p */
        uint64_t multiplier = 1;
        for (int k = 1; k <= e; ++k)
        {
            multiplier *= p;
            for (size_t j = 0; j < oldcnt; ++j)
            {
                divs[curcnt++] = divs[j] * multiplier;
            }
        }
    }

    /* Build proper divisors list and compute their sum */
    uint64_t *proper = (uint64_t *)calloc(totdiv, sizeof(uint64_t));
    size_t pscnt = 0;
    idx_t sumproper = 0;
    for (size_t i = 0; i < totdiv; ++i)
    {
        uint64_t d = divs[i];
        if (d == (uint64_t)n) continue;   /* skip n itself */
        proper[pscnt++] = d;
        sumproper += d;                   /* sum fits in idx_t for n <= 1e4 */
    }
    free(divs);

    /* Subset-sum DP over proper divisors */
    size_t sum = (size_t)sumproper;
    unsigned char *reach = (unsigned char *)calloc(sum + 1, sizeof(unsigned char));
    reach[0] = 1;
    for (size_t i = 0; i < pscnt; ++i)
    {
        idx_t d = proper[i];
        for (size_t s = sum; s >= (size_t)d; --s)
        {
            if (reach[s - (size_t)d]) reach[s] = 1;
        }
    }
    free(proper);

    /* Count reachable sums */
    size_t count = 0;
    for (size_t s = 0; s <= sum; ++s)
        if (reach[s]) ++count;

    /* Count duplicates: sums >= n that already exist without n */
    size_t dup = 0;
    if (n <= sum)
    {
        for (size_t s = (size_t)n; s <= sum; ++s)
            if (reach[s] && reach[s - (size_t)n]) ++dup;
    }

    free(reach);

    /* a(n) = count - dup */
    return (seq_t)(count - dup);
}
