/*
 * A378895  a(n) is n times the number of missing distinct digits, k, of the
 *          terms of the sequence so far, counted from start and thereafter
 *          from every term that immediately follows any term with a value of
 *          0.
 * https://oeis.org/A378895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * k_n where k_n is the number of decimal digits (0–9)
 *            that have not appeared in any a(1)…a(n‑1), counting from the
 *            start and after each term that equals 0. After a zero term all
 *            previously seen digits are forgotten. The sequence is generated
 *            recursively
 * Range:     u8 n <= 40, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Cache to allow arbitrary‑order calls */
static seq_t *cache   = NULL;   /* array of computed terms         */
static size_t cache_len = 0;    /* number of terms stored          */

/* Digits seen since the last zero value term */
static bool seen[10] = { false, false, false, false, false,
                         false, false, false, false, false };

/* Reset the seen array after a zero term */
static void reset_seen(void)
{
    for (int i = 0; i < 10; ++i)
        seen[i] = false;
}

/* Mark the decimal digits of x as seen */
static void add_digits(seq_t x)
{
    if (x == 0) {
        seen[0] = true;
        return;
    }
    while (x > 0) {
        int d = (int)(x % 10);
        seen[d] = true;
        x = x / 10;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* a(0) is undefined, return 0 as placeholder */

    /* Compute terms up to n if necessary */
    if ((idx_t)cache_len < n) {
        /* Allocate space for up to n terms */
        seq_t *new_cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (new_cache == NULL)
            return 0;   /* out of memory -- unlikely for n ≤ 10000 */

        /* Copy previously computed terms */
        if (cache_len > 0) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;

        /* Generate new terms */
        for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
            unsigned missing = 0;
            for (int d = 0; d < 10; ++d)
                if (!seen[d]) ++missing;

            seq_t term = ((seq_t)i) * ((seq_t)missing);
            cache[i - 1] = term;

            if (term == 0)
                reset_seen();
            else
                add_digits(term);
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
