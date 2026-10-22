/*
 * A379183  a(1)=0, a(2)=a(3)=1; a(n) = n - a(a(n-2)) for n>3.
 * https://oeis.org/A379183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; a(2)=a(3)=1; a(n) = n - a(a(n-2)) for n>3
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* memoized values, index 0 unused */
static unsigned char *computed = NULL;/* flag array: 1 if value computed */
static size_t cache_size = 0;        /* allocated size (elements including unused 0) */

static void grow_cache(idx_t needed) {
    if (needed < cache_size)
        return;
    size_t new_cap = cache_size ? cache_size << 1 : 16;
    if (new_cap <= needed) {
        /* increase until covering needed index */
        while (new_cap <= (size_t)needed)
            new_cap <<= 1;
    }
    /* allocate new arrays */
    seq_t *new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
    unsigned char *new_comp = (unsigned char*)calloc(new_cap, sizeof(unsigned char));
    if (new_cache == NULL || new_comp == NULL) {
        /* allocation failed – in this build we just abort */
        abort();
    }
    if (cache != NULL) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        memcpy(new_comp, computed, cache_size * sizeof(unsigned char));
        free(cache);
        free(computed);
    }
    cache = new_cache;
    computed = new_comp;
    cache_size = new_cap;
}

/* recursive with memoization */
seq_t a(idx_t n) {
    if (n == 0)  /* undefined for 0, but guard anyway */
        return 0;
    grow_cache(n);
    if (computed[n])
        return cache[n];

    seq_t result;
    if (n == 1) {
        result = 0;
    } else if (n == 2 || n == 3) {
        result = 1;
    } else {
        seq_t val_n2 = a(n - 2);
        idx_t inner = (idx_t)val_n2;
        seq_t val_inner = a(inner);
        result = (seq_t)n - val_inner;
    }

    cache[n] = result;
    computed[n] = 1;
    return result;
}
