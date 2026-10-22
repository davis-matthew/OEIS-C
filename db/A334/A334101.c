/*
 * A334101  Numbers of the form q*(2^k), where q is one of the Fermat primes
 *          and k >= 0; Numbers n for which A329697(n) == 1.
 * https://oeis.org/A334101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Numbers of the form q*2^k where q is a Fermat prime
 * (3, 5, 17, 257, 65537).  This implementation merges the five
 * geometric progressions q·2^k into a sorted list, caching terms
 * up to the requested index.  It works for any unsigned integer
 * type seq_t (8, 16, 32, 64 or arbitrary precision) and idx_t.'
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Known Fermat primes.  They are constant and small. */
static const seq_t fermat_primes[] = {
    (seq_t)3,
    (seq_t)5,
    (seq_t)17,
    (seq_t)257,
    (seq_t)65537
};
static const size_t NUM_FERMAT = 5;

/* Cached sequence values.  cache[n-1] == a(n). */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t last_gen   = 0;        /* number of terms currently cached */

/* Current value of each Fermat-prime progression q*2^k. */
static seq_t *curr = NULL;          /* allocated lazily */

/* Ensure that cache has space for at least (n) elements. */
static void ensure_cache_space(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (n > 16) ? n : 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < (size_t)n) {
        cache_cap = (n > cache_cap) ? n : (cache_cap << 1);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Ensure that the current progression array has been initialized. */
static void init_curr(void)
{
    if (curr == NULL) {
        curr = (seq_t *)malloc(NUM_FERMAT * sizeof(seq_t));
        for (size_t i = 0; i < NUM_FERMAT; ++i)
            curr[i] = fermat_primes[i];
    }
}

/* Generate the sequence up to (but not beyond) index n (1‑based). */
static void generate_up_to(idx_t n)
{
    if (n <= last_gen) return;      /* already generated */

    ensure_cache_space(n);
    init_curr();

    /* For 64‑bit or smaller types, compute a threshold to avoid UB
     * when left‑shifting.  For larger arbitrary‑precision types,
     * any shift is safe. */
    const int small_type = (int)sizeof(seq_t) <= 8;
    seq_t shift_limit = 0;
    if (small_type) { /* threshold = (1 << (bits-1)) - 1 */
        shift_limit = (((seq_t)1) << ((int)(sizeof(seq_t) * 8 - 1))) - 1;
    }

    while (last_gen < (size_t)n) {
        /* Find the minimal non‑zero current term. */
        seq_t  min_val = 0;
        size_t min_idx = 0;
        for (size_t i = 0; i < NUM_FERMAT; ++i) {
            if (curr[i] == 0) continue;   /* exhausted sequence */
            if (min_val == 0 || curr[i] < min_val) {
                min_val = curr[i];
                min_idx = i;
            }
        }
        if (min_val == 0) break;          /* no more terms (overflow) */

        cache[last_gen++] = min_val;       /* store the next term */

        /* Advance the chosen progression. */
        if (small_type) {
            if (curr[min_idx] <= shift_limit)
                curr[min_idx] <<= 1;       /* safe shift */
            else
                curr[min_idx] = 0;        /* would overflow: stop this line */
        } else {
            curr[min_idx] <<= 1;           /* arbitrary precision: safe */
        }
    }
}

/* Public function required by the testing framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1: a(1) is first term */
    generate_up_to(n);
    if ((size_t)(n - 1) < last_gen)
        return cache[n - 1];
    return 0;                              /* term does not fit in seq_t */
}
