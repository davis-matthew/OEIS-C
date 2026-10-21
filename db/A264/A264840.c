/*
 * A264840  Consider the sequence {3k, k >= 1}, and write down the numbers of
 *          consecutive terms that are squarefree.
 * https://oeis.org/A264840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the nth consecutive run of squarefree multiples
 *            of 3
 * Range:     u8 n <= 27, u16 n <= 7186, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

static seq_t *cache = NULL;      /* cached run lengths */
static size_t cache_len = 0;     /* number of runs cached */
static size_t cache_cap = 0;     /* current capacity of cache */
static idx_t next_k = 1;         /* next multiplier k for 3*k */
static seq_t run = 0;            /* current run length */

/* Ensure the run length cache has room for at least 'needed' entries. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, return 0 */

    /* generate runs until we have at least 'n' of them */
    while (cache_len < n) {
        seq_t m = (seq_t)3 * (seq_t)next_k;
        if (is_squarefree(m)) {
            run++;
        } else {
            if (run > 0) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = run;
                run = 0;
            }
        }
        next_k++;
    }
    return cache[n - 1];
}
