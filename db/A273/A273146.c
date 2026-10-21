/*
 * A273146  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 597", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 9, a(2) = 29, a(3) = 70, a(4) = 127, a(5) =
 *            231, a(6) = 347, a(7) = 540, a(8) = 733, for n > 8: a(n) =
 *            a(n‑1) + 2*a(n‑2) – 2*a(n‑3) – 2*a(n‑6) + 2*a(n‑7) + a(n‑8) –
 *            a(n‑9)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* dynamic memoisation table    */
static size_t cache_cap = 0;         /* last allocated size          */
static size_t last_computed = 0;     /* highest index computed so far */

seq_t a(idx_t n)
{
    /* first time: allocate at least 9 entries and initialise them */
    if (cache == NULL) {
        size_t init_cap = (size_t)n + 1;
        if (init_cap < 9)
            init_cap = 9;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache_cap = init_cap;

        /* initial six terms are known */
        const seq_t init[9] = { 1, 9, 29, 70, 127, 231, 347, 540, 733 };
        for (idx_t i = 0; i < 9; ++i)
            cache[(size_t)i] = init[i];
        last_computed = 8;
    }

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n if they are not yet known */
    if ((size_t)n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* compute positive part and negative part separately to avoid
               intermediate underflow in unsigned arithmetic */
            seq_t pos = cache[(size_t)(i - 1)];
            pos += 2 * cache[(size_t)(i - 2)];
            pos += 2 * cache[(size_t)(i - 7)];
            pos += cache[(size_t)(i - 8)];

            seq_t neg = 2 * cache[(size_t)(i - 3)];
            neg += 2 * cache[(size_t)(i - 6)];
            neg += cache[(size_t)(i - 9)];

            cache[(size_t)i] = pos - neg;
        }
        last_computed = n;
    }

    return cache[(size_t)n];
}
