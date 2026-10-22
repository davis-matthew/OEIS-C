/*
 * A370831  Alternating sum of composites.
 * https://oeis.org/A370831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4; a(n) = A002808(n) – a(n-1) for n>1 where A002808(n) is
 *            the n‑th composite number (4,6,8,9,10,12,…)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--- minimal deterministic primality test for 64‑bit values ---*/
static inline bool is_prime_uint64(idx_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    idx_t r = 0;
    for (idx_t d = 3; d * d <= v; d += 2)
        if (v % d == 0) return false;
    return true;
}

/*--- ensure capacity helper: doubles size or sets to needed ---*/
static void ensure_cap(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? (*cap * 2) : needed;
    if (new_cap < needed) new_cap = needed;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    *ptr = tmp;
    *cap = new_cap;
}

/*--- global state for composites and the sequence ---*/
static idx_t composites_cnt = 0;       /* number of stored composites (1‑based) */
static seq_t *composites    = NULL;    /* composites[1] … composites[composites_cnt] */
static size_t composites_cap = 0;
static idx_t next_check = 1;           /* integer to test for composites */

static idx_t a_cache_len = 0;          /* highest n for which a() is computed */
static seq_t *a_cache      = NULL;     /* a_cache[1] … a_cache[a_cache_len] */
static size_t a_cache_cap = 0;

/*--- ensure that composites[1 … n] exist ---*/
static void ensure_composites(idx_t n)
{
    while (composites_cnt < n) {
        /* find next composite number */
        do { ++next_check; } while (is_prime_uint64(next_check));

        /* store it (1‑based indexing) */
        ensure_cap(composites_cnt + 2, &composites_cap,
                   (void **)&composites, sizeof(seq_t));
        composites[composites_cnt + 1] = (seq_t)next_check;
        ++composites_cnt;
    }
}

/*--- sequence function ---*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not part of the OEIS offset */

    /* Gather enough composite numbers. */
    ensure_composites(n);

    /* Expand the cache if we need higher entries. */
    if (a_cache_len < n) {
        ensure_cap(n + 1, &a_cache_cap,
                   (void **)&a_cache, sizeof(seq_t));

        for (idx_t i = a_cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                a_cache[1] = composites[1];           /* a(1) = 4 */
            } else {
                a_cache[i] = composites[i] - a_cache[i - 1];
            }
        }
        a_cache_len = n;
    }

    return a_cache[n];
}
