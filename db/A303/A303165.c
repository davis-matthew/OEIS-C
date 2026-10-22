/*
 * A303165  Sum of the squarefree differences |q-p| of the parts in the
 *          partitions of n into two distinct parts (p,q) where p < q.
 * https://oeis.org/A303165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..⌊(n-1)/2⌋} (n−2i)·μ(n−2i)²
 * Range:     u8 n <= 36, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static memoisation state ------------------------------------------------- */

static seq_t *odd_sum  = NULL;   /* odd_sum[i]  = sum of odd squarefree numbers ≤ i */
static seq_t *even_sum = NULL;   /* even_sum[i] = sum of even squarefree numbers ≤ i */
static size_t max_calc = 0;      /* highest index that has been computed */
static uint8_t *isComposite = NULL;   /* sieve array (0 = prime, 1 = composite) */

/* Extend precomputation up to new_limit (inclusive). */
static void extend_to(size_t new_limit)
{
    if (new_limit <= max_calc)
        return;

    /* Resize composite sieve array. */
    size_t old_size = max_calc + 1;
    size_t new_size = new_limit + 1;

    uint8_t *tmp = (uint8_t *)realloc(isComposite, new_size * sizeof(uint8_t));
    if (tmp == NULL) abort();
    isComposite = tmp;
    /* initialise new entries as prime (0). */
    if (new_limit >= old_size)
        memset(isComposite + old_size, 0, new_limit - old_size + 1);

    /* Classic sieve to mark composites up to new_limit. */
    for (size_t p = 2; p * p <= new_limit; ++p) {
        if (!isComposite[p]) {
            for (size_t j = p * p; j <= new_limit; j += p)
                isComposite[j] = 1;
        }
    }

    /* Generate list of primes up to new_limit for marking square factors. */
    size_t prime_cap = new_limit / 10 + 10;
    uint64_t *prime_list = (uint64_t *)malloc((prime_cap + 1) * sizeof(uint64_t));
    if (prime_list == NULL) abort();
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (!isComposite[i]) {
            if (prime_cnt >= prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp2 = (uint64_t *)realloc(prime_list,
                                                    (prime_cap + 1) * sizeof(uint64_t));
                if (tmp2 == NULL) abort();
                prime_list = tmp2;
            }
            prime_list[prime_cnt++] = i;
        }
    }

    /* Create boolean array for squarefree numbers. */
    uint8_t *squarefree = (uint8_t *)malloc((new_limit + 1) * sizeof(uint8_t));
    if (squarefree == NULL) abort();
    memset(squarefree, 1, new_limit + 1);   /* assume all squarefree initially */
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p  = prime_list[idx];
        uint64_t p2 = p * p;
        if (p2 > new_limit)
            break;
        for (uint64_t j = p2; j <= new_limit; j += p2)
            squarefree[(size_t)j] = 0;
    }
    free(prime_list);

    /* Resize prefix sum arrays. */
    seq_t *tmp2 = (seq_t *)realloc(odd_sum,  new_size * sizeof(seq_t));
    if (tmp2 == NULL) abort();
    odd_sum = tmp2;
    tmp2 = (seq_t *)realloc(even_sum, new_size * sizeof(seq_t));
    if (tmp2 == NULL) abort();
    even_sum = tmp2;

    /* Compute prefix sums for odd and even squarefree numbers. */
    odd_sum[0]  = 0;
    even_sum[0] = 0;
    for (size_t i = 1; i <= new_limit; ++i) {
        odd_sum[i]  = odd_sum[i-1];
        even_sum[i] = even_sum[i-1];
        if (squarefree[i]) {
            if (i & 1)
                odd_sum[i] += (seq_t)i;   /* odd i */
            else
                even_sum[i] += (seq_t)i;  /* even i */
        }
    }
    free(squarefree);
    max_calc = new_limit;
}

/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* For n = 1, 2 the summation is empty. */
    if (n <= 2)
        return (seq_t)0;

    /* limit = n-2; guard against wraparound if n < 3. */
    size_t limit = (size_t)(n - 2);

    extend_to(limit);

    /* Sum over numbers of the same parity as n up to limit. */
    return (n & 1) ? odd_sum[limit] : even_sum[limit];
}
