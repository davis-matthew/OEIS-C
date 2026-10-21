/*
 * A287002  Start with 0 and repeatedly substitute 0->01, 1->20, 2->1.
 * https://oeis.org/A287002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol of the infinite word fixed by the *
 *            morphism 0 → 01, 1 → 20, 2 → 1, starting from the single symbol
 *            0. * The routine builds the word incrementally, extending a
 *            static cache * until it contains at least n symbols. Each
 *            extension expands the * current word by applying the sub...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;  /* array holding the generated word */
static idx_t seq_len  = 0;       /* number of symbols stored in seq_cache */

/* Extend the cache so that it contains at least n symbols (1‑based index). */
static void ensure_len(idx_t n)
{
    if (seq_len >= n)
        return;  /* already long enough */

    /* On first call initialise with the starting symbol 0. */
    if (!seq_cache) {
#ifdef __cplusplus
        seq_cache = new seq_t[1];
#else
        seq_cache = (seq_t *)malloc(sizeof(seq_t));
#endif
        seq_cache[0] = 0;
        seq_len = 1;
    }

    /* Repeatedly apply the substitution until the cache is long enough. */
    while (seq_len < n) {
        size_t old = (size_t)seq_len;
        size_t new_len = 0;
        /* Count the length after one substitution. */
        for (size_t i = 0; i < old; ++i) {
            seq_t v = seq_cache[i];
            if (v == 0 || v == 1)
                new_len += 2;
            else /* v == 2 */
                new_len += 1;
        }

        /* Allocate new storage. */
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_len];
#else
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
#endif

        /* Perform the substitution into the new array. */
        size_t pos = 0;
        for (size_t i = 0; i < old; ++i) {
            seq_t v = seq_cache[i];
            if (v == 0) {          /* 0 → 0 1 */
                tmp[pos++] = 0;
                tmp[pos++] = 1;
            } else if (v == 1) {   /* 1 → 2 0 */
                tmp[pos++] = 2;
                tmp[pos++] = 0;
            } else {               /* 2 → 1 */
                tmp[pos++] = 1;
            }
        }

        /* Free the old cache. */
#ifdef __cplusplus
        delete[] seq_cache;
#else
        free(seq_cache);
#endif

        seq_cache = tmp;
        seq_len = new_len;
    }
}

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    if (n == 0)      /* out‑of‑range guard; offset is 1 */
        return 0;
    ensure_len(n);
    return seq_cache[n - 1];
}
