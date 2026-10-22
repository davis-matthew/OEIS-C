/*
 * A378384  Digital root of the sum of the previous 3 terms; a(0) = a(1) =
 *          a(2) = 1.
 * https://oeis.org/A378384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root(a(n-1)+a(n-2)+a(n-3)) a(0) = a(1) = a(2) = 1
 *            (Digital root: if sum % 9 == 0 return 9, else return sum % 9)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for sequence values */
static seq_t *cache = NULL;   /* array of computed terms              */
static size_t cache_cap = 0;  /* allocated capacity (in terms)        */
static size_t cache_len = 0;  /* number of terms actually stored      */

/* compute digital root of a non‑negative seq_t value
   (safe for any seq_t that implements % and ==) */
static seq_t digital_root(seq_t x)
{
    if (x == 0) return 0;
    seq_t r = x % 9;
    return (r == 0) ? 9 : r;
}

seq_t a(idx_t n)
{
    /* extend the cache if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (new_cap < needed) new_cap *= 2;

        seq_t *new_buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_buf) exit(1);                    /* allocation failure */

        if (cache != NULL) {
            for (size_t i = 0; i < cache_len; ++i) new_buf[i] = cache[i];
            free(cache);
        }
        cache     = new_buf;
        cache_cap = new_cap;
    }

    /* generate missing terms up to index n */
    while (cache_len <= n) {
        seq_t val;
        if (cache_len < 3) {
            val = 1;                 /* initial tribonacci values */
        } else {
            /* compute digital root of the sum without overflow
               by adding the individual mods modulo 9 */
            seq_t m0 = cache[cache_len - 3] % 9;
            seq_t m1 = cache[cache_len - 2] % 9;
            seq_t m2 = cache[cache_len - 1] % 9;
            seq_t sum_mod9 = (m0 + m1 + m2) % 9;
            val = (sum_mod9 == 0) ? 9 : sum_mod9;
        }
        cache[cache_len++] = val;
    }

    return cache[n];
}
