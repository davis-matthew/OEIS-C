/*
 * A355166  Lexicographically earliest sequence of distinct positive integers
 *          such for any n > 0, n and a(n) are coprime and have no common
 *          1-bits in their binary expansions.
 * https://oeis.org/A355166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the lexicographically earliest permutation
 * of the positive integers such that for every n > 0
 * (a(n), n) are coprime and have no common 1‑bits in binary. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *mapping = NULL;   /* mapping[i] = a(i), 0 if unset   */
static size_t capacity = 0;     /* current capacity of mapping[]   */

/* enlarge the mapping array so that index n is available (1‑based) */
static void ensure_size(idx_t n)
{
    if ((idx_t)capacity > n) return;        /* already large enough   */
    size_t new_cap = capacity ? capacity : 1;
    while ((size_t)n >= new_cap) new_cap <<= 1;

#if defined(__cplusplus)
    seq_t *new_map = new seq_t[new_cap];
    if (mapping) {
        for (size_t i = 0; i < capacity; ++i) new_map[i] = mapping[i];
        delete [] mapping;
    }
    mapping = new_map;
    capacity = new_cap;
#else
    seq_t *new_map = realloc(mapping, new_cap * sizeof(seq_t));
    if (!new_map) abort();                     /* allocation failure */
    for (size_t i = capacity; i < new_cap; ++i) new_map[i] = 0;
    mapping = new_map;
    capacity = new_cap;
#endif
}

/* generate the permutation up to and including target */
static void generate_up_to(idx_t target)
{
    static idx_t last_done = 0;          /* last index processed   */
    if (target <= last_done) return;

    for (idx_t i = last_done + 1; i <= target; ++i) {
        ensure_size(i);
        if (mapping[i] != 0) continue;   /* already paired          */

        idx_t m = 1;
        for (;;) {
            ensure_size(m);
            if (m == i) { ++m; continue; }
            if (mapping[m] != 0) { ++m; continue; }
            if (((seq_t)i & (seq_t)m) != 0) { ++m; continue; }
            if (gcd((seq_t)i, (seq_t)m) != 1) { ++m; continue; }

            mapping[i] = (seq_t)m;
            mapping[m] = (seq_t)i;
            break;
        }
    }
    last_done = target;
}

/* external function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    generate_up_to(n);
    return mapping[n];
}
