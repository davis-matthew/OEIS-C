/*
 * A255342  Numbers n such that there are exactly two 1's in their factorial
 *          base representation (A007623).
 * https://oeis.org/A255342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search numbers with exactly two 1's in their factorial base
 *            representation
 * Range:     u8 n <= 90, u16 n <= 13132, u32 n <= 13132, u64 n <= 13132,
 *            u128 n <= 13132, big any n
 * Validated: standard, pass, 84 terms through n = 9516 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* count_ones_factorial_base returns the number of digits equal to 1 in the
   factorial base representation of value.  It works for any seq_t in the
   8‑, 16‑, 32‑, 64‑, 128‑bit or arbitrary‑precision builds. */
static size_t count_ones_factorial_base(seq_t value) {
    if (value == 0) return 0;

    /* Build factorials up to the largest that does not exceed value.
       32 factorials are more than enough for any reachable value in the
       64‑bit or 128‑bit range relevant to the first 10 000 terms. */
    seq_t facts[32];
    size_t fc = 0;

    seq_t f = 1;
    facts[fc++] = f;           /* 1! */

    size_t k = 2;
    while (fc < 32) {
        if (f > value / (seq_t)k) break;    /* next factorial would exceed value or overflow */
        f *= (seq_t)k;
        facts[fc++] = f;
        if (f > value) break;
        ++k;
    }

    /* Determine the highest factorial position needed for the representation */
    size_t start = (fc == 0) ? 0 : fc - 1;
    if (facts[start] > value) {
        if (start > 0) start--;
    }

    /* Now walk from the highest position downwards, counting digits equal to 1 */
    size_t count = 0;
    seq_t rem = value;
    for (int i = (int)start; i >= 0; --i) {
        seq_t digit = rem / facts[i];
        rem %= facts[i];
        if (digit == 1) count++;
    }

    return count;
}

/* a returns the n-th member of A255342. */
seq_t a(idx_t n) {
    /* Static cache to answer any sequence of calls in O(1) after the
       initial construction of the required prefix. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t last_checked = 0;   /* last candidate examined */

    if (n == 0) return 0;           /* undefined, but guard against underflow */

    if ((idx_t)cache_size >= n) {
        return cache[n - 1];         /* already computed */
    }

    /* Expand cache until we have at least n terms. */
    while (cache_size < (size_t)n) {
        ++last_checked;                     /* next integer to examine */

        if (count_ones_factorial_base(last_checked) == 2) {
            /* Grow cache if needed */
            if (cache_size >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }

    return cache[n - 1];
}
