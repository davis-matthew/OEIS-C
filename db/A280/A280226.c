/*
 * A280226  Number of partitions of 2n into two squarefree parts.
 * https://oeis.org/A280226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} mu(i)^2 * mu(2n-i)^2 * where mu(k)^2 equals
 *            1 if k is squarefree, otherwise 0. * The routine builds a sieve
 *            of square multiples and counts pairs
 * Range:     u8 n <= 449, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint8_t *sqfree = NULL;   /* 1 = squarefree, 0 = not squarefree */
static size_t sqfree_cap = 0;    /* current capacity of the sieve */

/* floor(sqrt(x)) for size_t */
static size_t floor_sqrt(size_t x)
{
    size_t lo = 0, hi = x;
    while (lo < hi) {
        size_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Ensure sqfree[0 .. need-1] is correct for numbers up to "need-1". */
static void ensure_sieve(size_t need)
{
    if (sqfree_cap >= need)
        return;  /* already sufficient */

    free(sqfree);
    sqfree = (uint8_t*)calloc(need, sizeof(uint8_t));
    if (!sqfree)
        return;  /* allocation failed – treat as all squarefree (unlikely) */

    /* Initially assume all numbers are squarefree */
    for (size_t i = 0; i < need; ++i)
        sqfree[i] = 1;

    /* Sieve out numbers divisible by a perfect square */
    size_t limit = need - 1;
    size_t sqrt_limit = floor_sqrt(limit);

    /* Prime sieve up to sqrt_limit */
    uint8_t *is_prime = (uint8_t*)calloc(sqrt_limit + 1, sizeof(uint8_t));
    if (!is_prime) {
        free(sqfree);
        sqfree = NULL;
        return;
    }
    for (size_t p = 2; p <= sqrt_limit; ++p)
        is_prime[p] = 1;
    for (size_t p = 2; p * p <= sqrt_limit; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= sqrt_limit; q += p)
                is_prime[q] = 0;
        }
    }

    for (size_t p = 2; p <= sqrt_limit; ++p) {
        if (!is_prime[p])
            continue;
        size_t sq = p * p;
        for (size_t k = sq; k < need; k += sq)
            sqfree[k] = 0;
    }

    free(is_prime);
    sqfree_cap = need;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset is 1; below 1 is irrelevant */

    size_t need = (size_t)2 * (size_t)n + 1;  /* indices up to 2n inclusive */
    ensure_sieve(need);

    seq_t count = 0;
    size_t two_n = (size_t)2 * (size_t)n;
    for (idx_t i = 1; i <= n; ++i) {
        if (sqfree[(size_t)i] && sqfree[two_n - (size_t)i])
            ++count;
    }
    return count;
}
