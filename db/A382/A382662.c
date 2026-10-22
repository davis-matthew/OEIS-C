/*
 * A382662  The unitary totient function applied to the cubefree numbers
 *          (A004709).
 * https://oeis.org/A382662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A047994(A004709(n)) unitary totient of the n‑th cubefree
 *            integer
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/factor_u64.h"

/* Cache of cubefree numbers, 1‑based. */
static seq_t *cubefree_cache = NULL;
static size_t cubefree_cap   = 0;
static size_t cubefree_cnt   = 0;
static uint64_t next_candidate = 1;   /* next integer to test */

/* Ensure the cache can hold at least `needed` elements. */
static void grow_cubefree_cap(size_t needed)
{
    if (cubefree_cap < needed) {
        size_t new_cap = cubefree_cap ? cubefree_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *newptr = (seq_t *)realloc(cubefree_cache, new_cap * sizeof(seq_t));
        assert(newptr != NULL);
        cubefree_cache = newptr;
        cubefree_cap = new_cap;
    }
}

/* Generate cubefree numbers until we have at least n of them. */
static void generate_cubefree_until(idx_t n)
{
    while (cubefree_cnt < n) {
        uint64_t x = next_candidate++;
        uint64_t pr[32], ex[32];
        size_t cnt = 0;
        factor_u64(x, pr, ex, &cnt);
        bool ok = true;
        for (size_t i = 0; i < cnt; ++i) {
            if (ex[i] > 2) { ok = false; break; }
        }
        if (ok) {
            grow_cubefree_cap(cubefree_cnt + 1);
            cubefree_cache[cubefree_cnt++] = (seq_t)x;
        }
    }
}

/* Return the n‑th cubefree number (1‑based). */
static uint64_t nth_cubefree(idx_t n)
{
    assert(n >= 1);
    generate_cubefree_until(n);
    return (uint64_t)cubefree_cache[n - 1];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    uint64_t m = nth_cubefree(n);

    /* Factorise m. */
    uint64_t pr[32], ex[32];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        uint64_t e = ex[i];
        seq_t p_pow = 1;
        for (uint64_t k = 0; k < e; ++k) p_pow *= p;
        result *= (p_pow - 1);
    }
    return result;
}
