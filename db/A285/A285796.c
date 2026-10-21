/*
 * A285796  Number of ways to write n as an ordered sum of two numbers that
 *          are the product of an even number of distinct primes (including
 *          1).
 * https://oeis.org/A285796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (x, n‑x) with x and n‑x
 *            square‑free and having an even number of distinct prime factors
 *            (1 qualifies)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache of smallest prime factors and qualification flag. */
static uint32_t *spf_table = NULL;   /* spf_table[i] = smallest prime factor of i (0 if i <= 1) */
static uint8_t  *qual_table = NULL;  /* 1 if i satisfies the condition, 0 otherwise */
static size_t   cache_limit = 0;     /* largest n for which cache is valid */

/* Build or rebuild the cache up to 'limit' (inclusive). */
static void build_cache(idx_t limit)
{
    size_t n = (size_t)limit;
    if (n <= cache_limit) return;  /* already up to date */

    /* Allocate new tables. */
    uint32_t *new_spf = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
    uint8_t  *new_qual = (uint8_t *)calloc(n + 1, sizeof(uint8_t));
    if (!new_spf || !new_qual) {
        /* Allocation failed – keep old cache if any. */
        return;
    }

    /* Free old tables if they exist. */
    free(spf_table);
    free(qual_table);

    spf_table = new_spf;
    qual_table = new_qual;

    /* Sieve for smallest prime factors. */
    for (size_t i = 2; i <= n; ++i) {
        if (spf_table[i] == 0) {            /* i is prime */
            spf_table[i] = (uint32_t)i;
            if (i <= 46340 && i * i <= n) {   /* i*i fits in 32‑bit */
                for (size_t j = i * i; j <= n; j += i)
                    if (spf_table[j] == 0)
                        spf_table[j] = (uint32_t)i;
            }
        }
    }

    /* Qualification flag: square‑free and even number of distinct primes. */
    qual_table[1] = 1;   /* 1 is square‑free with 0 primes (even) */
    for (size_t i = 2; i <= n; ++i) {
        size_t t = i;
        size_t last = 0;
        size_t cnt = 0;
        bool squarefree = true;
        while (t > 1) {
            size_t p = spf_table[t];
            if (p == last) {          /* repeated prime factor → not square‑free */
                squarefree = false;
                break;
            }
            ++cnt;
            last = p;
            t /= p;
        }
        qual_table[i] = (squarefree && ((cnt & 1) == 0)) ? 1 : 0;
    }

    cache_limit = n;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n <= 1) return 0; /* No ordered pair of positive parts. */

    /* Ensure cache covers n. */
    if (cache_limit < (size_t)n)
        build_cache(n);

    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x) {
        if (qual_table[(size_t)x] && qual_table[(size_t)(n - x)])
            ++count;
    }
    return count;
}
