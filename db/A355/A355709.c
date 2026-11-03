/*
 * A355709  Numbers k such that k and k+1 have the same number of 3-smooth
 *          divisors.
 * https://oeis.org/A355709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 8737, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9841 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A355709: numbers k such that k and k+1 have the same number of
   3‑smooth divisors.
   Computed by brute‑force search: find k with (e2(k)+1)*(e3(k)+1) ==
   (e2(k+1)+1)*(e3(k+1)+1), where e2 and e3 are the exponents of 2 and 3 in n. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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

static void exponents(seq_t n, idx_t *v2, idx_t *v3)
{
    seq_t t = n;
    idx_t e2 = 0;
    while ((t & 1) == 0) {
        t >>= 1;
        ++e2;
    }
    idx_t e3 = 0;
    while ((t % 3) == 0) {
        t /= 3;
        ++e3;
    }
    *v2 = e2;
    *v3 = e3;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_k = 1;

    while (cache_len < n) {
        seq_t k  = next_k;
        seq_t k1 = k + 1;

        idx_t e2k, e3k, e2k1, e3k1;
        exponents(k,  &e2k, &e3k);
        exponents(k1, &e2k1,&e3k1);

        seq_t cntk  = ((seq_t)(e2k + 1)) * ((seq_t)(e3k + 1));
        seq_t cntk1 = ((seq_t)(e2k1 + 1)) * ((seq_t)(e3k1 + 1));

        if (cntk == cntk1) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = k;
        }
        next_k++;
    }

    return cache[n - 1];
}
