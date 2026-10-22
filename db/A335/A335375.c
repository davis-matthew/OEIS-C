/*
 * A335375  Numbers k such that the k-th composition in standard order
 *          (A066099) is neither unimodal nor co-unimodal.
 * https://oeis.org/A335375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer k such that the k‑th composition in
 *            the graded reverse‑lexicographic standard order (A066099) is
 *            neither unimodal nor co‑unimodal. The function generates
 *            compositions in that order and selects the indices that fail the
 *            unimodal/co‑unimodal test
 * Range:     u8 n <= 43, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_valley
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_valley.h"

/* floor(log2(x)) for x>0 */
static inline idx_t floor_log2_uint64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(x);
#else
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
#endif
}

/* Check whether parts[0..len-1] form a unimodal (mountain) composition. */
static bool is_unimodal(const unsigned int *parts, size_t len)
{
    size_t i = 0;
    while (i + 1 < len && parts[i] <= parts[i + 1]) ++i;
    while (i + 1 < len && parts[i] >= parts[i + 1]) ++i;
    return i + 1 == len;
}

/* Construct the composition corresponding to the absolute composition
   index k (1‑based). The resulting parts are stored in 'parts' and
   its length in '*len'. */
static void composition_of_k(idx_t k, unsigned int *parts, size_t *len)
{
    idx_t n = floor_log2_uint64(k) + 1;   /* sum of the composition */
    if (n == 1) {                         /* only (1) */
        *len = 1;
        parts[0] = 1;
        return;
    }

    idx_t first = ((idx_t)1 << (n - 1));
    idx_t offset = k - first;             /* 0‑based index within n‑compositions */
    size_t bits = n - 1;
    unsigned int part = 1;
    *len = 0;

    for (size_t i = 0; i < bits; ++i) {
        int bit = (offset >> (bits - 1 - i)) & 1;   /* most significant bit first */
        if (bit) {
            parts[(*len)++] = part;
            part = 1;
        } else {
            ++part;
        }
    }
    parts[(*len)++] = part;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but we never call with 0 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;       /* next composition index to process */

    /* Generate terms until cache has n entries */
    while (cache_len < (size_t)n) {
        /* Grow cache if necessary */
        if (cache_cap <= cache_len) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 1;
            while (new_cap <= cache_len) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        unsigned int parts[128];
        size_t parts_len;
        composition_of_k(next_k, parts, &parts_len);

        if (!is_unimodal(parts, parts_len) && !is_valley(parts, parts_len))
            cache[cache_len++] = (seq_t)next_k;

        ++next_k;
    }

    return cache[n - 1];
}
