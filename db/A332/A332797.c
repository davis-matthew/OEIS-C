/*
 * A332797  Numbers whose smallest prime factor is 23.
 * https://oeis.org/A332797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*A166063(n)
 * Range:     u8 n <= 1, u16 n <= 484, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Primes smaller than 23 */
static const idx_t primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
static const size_t NUM_PRIMES = sizeof(primes) / sizeof(primes[0]);

/* Cached sequence terms */
static seq_t *terms     = NULL;
static size_t terms_len = 0;   /* number of terms currently cached */
static size_t terms_cap = 0;   /* allocated capacity */
static idx_t  last_m    = 0;   /* last integer 'm' that was examined */

/* Ensure that the cache can hold at least 'need' terms. */
static void grow_terms(idx_t need)
{
    if ((idx_t)terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while ((idx_t)new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                 /* abort on allocation failure */
    terms = tmp;
    terms_cap = new_cap;
}

/* Expand the cache until it contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (terms_len >= n) return;

    for (idx_t m = last_m + 1; terms_len < n; ++m)
    {
        int ok = 1;
        for (size_t i = 0; i < NUM_PRIMES; ++i)
        {
            if ((m % primes[i]) == 0)
            {
                ok = 0;
                break;
            }
        }

        if (ok)
        {
            grow_terms(terms_len + 1);
            seq_t value = (seq_t)23 * m;
            terms[terms_len++] = value;
        }

        last_m = m;   /* remember the last processed 'm' */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* the sequence is defined for n ≥ 1 */
    ensure_terms(n);
    return terms[n - 1];                /* OEIS offset is 1 */
}
