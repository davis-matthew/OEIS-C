/*
 * A354833  a(1)=1; for n > 1, a(n) = a(n-1) - n unless that result is already
 *          in the sequence or would be negative; otherwise, a(n) = a(n-1) *
 *          n.
 * https://oeis.org/A354833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) - n if that result is
 *            non‑negative, not already in the sequence, otherwise a(n) =
 *            a(n-1) * n
 * Range:     u8 n <= 30, u16 n <= 214, u32 n <= 9373, u64 n <= 9373,
 *            u128 n <= 9373, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation notes:
   - We memoise computed values in a static array.
   - For membership testing we use a linear scan over the existing values.
   - The algorithm runs in O(n²) worst‑case time, which is easily fast enough
     for n up to about 10000.  */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cache_len = 0;     /* number of terms stored */
static idx_t  cache_cap = 0;     /* allocated capacity of the array */

/* Ensure cache contains terms up to 'upto' (inclusive). */
static void
ensure_size(idx_t upto)
{
    if (upto == 0) return;               /* no work for n == 0 */
    if (upto <= cache_len) return;       /* already computed */

    /* Allocate / resize the array if necessary */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();  /* out of memory */
    }
    if (upto > cache_cap) {
        while (cache_cap < upto) cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Initialise the very first term if this is the first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(1) */
        cache_len = 1;
    }

    /* Compute remaining terms up to 'upto' */
    for (idx_t i = cache_len + 1; i <= upto; ++i) {
        seq_t prev = cache[cache_len - 1];
        seq_t new_val;
        bool can_subtract = false;

        if (prev >= (seq_t)i) {
            seq_t cand = prev - (seq_t)i;
            /* Check if cand is already in the sequence */
            bool used = false;
            for (idx_t j = 0; j < cache_len; ++j) {
                if (cache[j] == cand) {
                    used = true;
                    break;
                }
            }
            can_subtract = !used;
            if (can_subtract) new_val = cand;
        }

        if (!can_subtract) {
            new_val = prev * (seq_t)i;
        }

        cache[cache_len++] = new_val;
    }
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined in OEIS but safe guard */
    ensure_size(n);
    return cache[n - 1];
}
