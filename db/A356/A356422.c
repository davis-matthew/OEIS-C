/*
 * A356422  Heptagonal numbers which are products of three distinct primes.
 * https://oeis.org/A356422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 945 (2026-11-03)
 * Helpers:   is_squarefree, mem, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Heptagonal numbers that are products of three distinct primes
   (OEIS sequence A356422); the values are found by searching the
   heptagonal number sequence H_k = k*(5*k-3)/2 and keeping only those
   that are square‑free and have exactly three distinct prime factors. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/omega.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_k = 0;              /* last k examined */
    if (n == 0) return 0;                 /* behaviour undefined for n<1 */

    /* keep generating terms until we have at least n terms in cache */
    while (cache_len < (size_t)n) {
        ++last_k;                         /* next index k */
        seq_t k = (seq_t)last_k;
        seq_t Hk = k * (((seq_t)5 * last_k) - 3) / 2;   /* k*(5k-3)/2 */

        if (is_squarefree(Hk) &&
            omega((uint64_t)Hk) == 3) {
            if (cache_len >= cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = Hk;
        }
    }
    return cache[n-1];
}
