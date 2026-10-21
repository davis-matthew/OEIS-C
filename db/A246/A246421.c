/*
 * A246421  Numbers n such that (n + digit sum of n) and (n + digit product of
 *          n) are nontrivial permutations of the digits of n.
 * https://oeis.org/A246421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n such that * n + digit_sum(n) and n +
 *            digit_product(n) * are non‑trivial permutations of the digits of
 *            n. * The function uses lazy, incremental search with memoisation
 * Range:     u8 none, u16 n <= 6, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   digit_frequencies, digit_len, digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_frequencies.h"
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/digit_len.h"

static seq_t *cache = NULL;     /* array of found terms */
static size_t cache_len = 0;    /* number of cached terms */
static idx_t next_candidate = 1;/* next integer to examine */

/* Append a new value to the cache, growing it if necessary */
static void cache_append(seq_t v)
{
    if (cache_len == 0)
    {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) abort();
    }
    else if (cache_len % 2 == 0)
    {
        cache = (seq_t *)realloc(cache, (cache_len * 2) * sizeof(seq_t));
        if (!cache) abort();
    }
    cache[cache_len++] = v;
}

/* Test whether candidate satisfies the definition */
static bool satisfies(seq_t cand)
{
    unsigned int freq_c[10];
    digit_frequencies(cand, freq_c);

    seq_t sum = digit_sum(cand);
    seq_t prod = digit_product(cand);

    /* a product of zero cannot give a non‑trivial permutation */
    if (prod == 0)
        return false;

    seq_t x1 = cand + sum;
    seq_t x2 = cand + prod;

    /* all three numbers must have the same number of digits */
    int len = digit_len(cand);
    if (digit_len(x1) != len || digit_len(x2) != len)
        return false;

    unsigned int freq_x1[10];
    digit_frequencies(x1, freq_x1);
    for (int d = 0; d < 10; ++d)
        if (freq_c[d] != freq_x1[d])
            return false;
    if (x1 == cand)
        return false; /* permutation must be non‑trivial */

    unsigned int freq_x2[10];
    digit_frequencies(x2, freq_x2);
    for (int d = 0; d < 10; ++d)
        if (freq_c[d] != freq_x2[d])
            return false;
    if (x2 == cand)
        return false;

    return true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset, return 0 */

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        seq_t cand = (seq_t)next_candidate;
        if (satisfies(cand))
            cache_append(cand);
        ++next_candidate;
    }
    return cache[n - 1];
}
