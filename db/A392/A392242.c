/*
 * A392242  Squares with distinct digits.
 * https://oeis.org/A392242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) = square of the (n-1)th integer whose square has distinct
 *            digits
 * Range:     u8 n <= 12, u16 n <= 106, u32 n <= 566, u64 n <= 611,
 *            u128 n <= 611, big any n
 * Validated: full, pass, 611 terms through n = 611 (2026-11-03)
 * Helpers:   digit_counts, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/digit_counts.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t term_count = 0;
    static idx_t next_candidate = 0;

    while (term_count < n) {
        seq_t base = (seq_t)next_candidate;
        seq_t sq   = seq_mul(base, base);

        unsigned char freq[10] = {0};
        digit_counts(sq, freq);

        bool distinct = true;
        for (int d = 0; d < 10; ++d) {
            if (freq[d] > 1) {
                distinct = false;
                break;
            }
        }

        if (distinct) {
            if (cap <= term_count) {
                ensure_capacity(term_count + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[term_count++] = sq;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
