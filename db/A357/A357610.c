/*
 * A357610  Start with x = 3 and repeat the map x -> floor(n/x) + (n mod x)
 *          until an x occurs that has already appeared, then that is a(n).
 * https://oeis.org/A357610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 765, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A357610: For a given n > 0, set x₀ = 3. Then
 *      xₖ₊₁ = ⌊n / xₖ⌋ + (n % xₖ).
 *  The sequence continues until a value repeats; the first repeated
 *  value is a(n).  This implements OEIS A357610.
 */

#include <stdlib.h>
#include <stddef.h>
#if defined(__cplusplus)
#  /* C++ already has bool */
#else
#  include <stdbool.h>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n = 0 – safe guard */
    idx_t cur = 3;

    /* Fast path for small n: use a contiguous bool array */
    if (n <= 1000000ULL && (size_t)n <= SIZE_MAX - 5) {
        size_t sz = (size_t)n + 5;           /* +5 for indices 0 … n+4 */
        bool *visited = (bool*)calloc(sz, sizeof(bool));
        if (visited) {
            visited[cur] = true;            /* mark the starting value */
            while (1) {
                idx_t nxt = (n / cur) + (n % cur);
                if (visited[(size_t)nxt]) {
                    free(visited);
                    return (seq_t)nxt;      /* first repeated value found */
                }
                visited[(size_t)nxt] = true;
                cur = nxt;
            }
        }
    }

    /* Fallback: dynamic array (vector) for visited values – works in C */
    size_t seen_cap = 4;
    idx_t *seen = (idx_t*)malloc(seen_cap * sizeof(idx_t));
    if (!seen) {                               /* unlikely OOM – return current */
        return (seq_t)cur;
    }
    seen[0] = cur;                             /* seed the set */
    size_t seen_len = 1;

    while (1) {
        idx_t nxt = (n / cur) + (n % cur);

        /* Linear search for the next value */
        size_t i;
        for (i = 0; i < seen_len; ++i) {
            if (seen[i] == nxt) {
                free(seen);
                return (seq_t)nxt;            /* repeat detected */
            }
        }

        /* Grow the array if necessary */
        if (seen_len == seen_cap) {
            seen_cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(seen, seen_cap * sizeof(idx_t));
            if (!tmp) {                       /* OOM – abort gracefully */
                free(seen);
                return (seq_t)nxt;
            }
            seen = tmp;
        }

        seen[seen_len++] = nxt;               /* record the new value */
        cur = nxt;
    }
}
