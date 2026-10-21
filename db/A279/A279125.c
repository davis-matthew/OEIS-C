/*
 * A279125  Lexicographically earliest sequence such that, for any distinct i
 *          and j, a(i)=a(j) implies (i AND j)=0 (where AND stands for the
 *          bitwise AND operator).
 * https://oeis.org/A279125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 406, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence with the property:
   for any distinct i,j, a(i)=a(j) ⇒ (i & j) == 0.
   Greedy construction: each value v keeps a bitwise OR (mask)
   of all indices assigned to v.  An index n can be assigned
   to v iff (n & mask[v]) == 0.  The first such v is chosen,
   giving the lexicographically earliest sequence. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache      = NULL;   /* cache[1..] holds a(i) values          */
static uint32_t *vmask   = NULL;   /* bitwise OR of indices per value       */
static idx_t last_computed = 0;    /* largest index computed so far         */
static size_t cache_cap = 0;       /* capacity of cache array              */
static size_t vmask_cap = 0;       /* capacity of vmask array              */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, undefined for 0 */

    /* Grow cache array to hold up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        for (size_t v = 0; ; ++v) {
            /* Ensure vmask array covers value v */
            if (v >= vmask_cap) {
                size_t new_cap = v + 1;
                uint32_t *tmp = (uint32_t *)realloc(vmask, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                for (size_t j = vmask_cap; j < new_cap; ++j) tmp[j] = 0;
                vmask = tmp;
                vmask_cap = new_cap;
            }

            /* Test whether index i can be assigned the value v */
            if ((i & (idx_t)vmask[v]) == 0) {
                cache[i] = (seq_t)v;
                vmask[v] |= (uint32_t)i;
                break;
            }
        }
    }
    last_computed = n;
    return cache[n];
}
