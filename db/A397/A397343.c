/*
 * A397343  Happy numbers whose trajectories under iteration of the sum of
 *          squares of digits map to 100 before going to the value of 1.
 * https://oeis.org/A397343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 5429, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_sq_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by filtering integers that reach 100 before 1 under
   the sum‑of‑squares‑of‑digits map. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sum_sq_digits_uint64.h"

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*  table of size 2001: for every n <= 2000 we precompute whether
    the trajectory from n contains 100 before reaching 1.          */
#define TABLE_MAX 2000
static bool hit100[TABLE_MAX + 1];
static bool table_ready = false;

static void init_table(void)
{
    if (table_ready) return;
    for (int i = 0; i <= TABLE_MAX; ++i) {
        bool visited[TABLE_MAX + 1] = {false};
        int cur = i;
        while (1) {
            if (cur == 100) { hit100[i] = true; break; }
            if (cur == 1 || cur == 0) { hit100[i] = false; break; }
            if (visited[cur]) { hit100[i] = false; break; }
            visited[cur] = true;
            cur = (int)sum_sq_digits_uint64((uint64_t)cur);
        }
    }
    table_ready = true;
}

/* ------------------------------------------------------------ */
/*  does the number n hit 100 before it hits 1 under repeated
    sum‑of‑squares‑of‑digits iteration?                           */
static bool hits_100_before_1(uint64_t n)
{
    init_table();
    while (n > TABLE_MAX) {
        n = sum_sq_digits_uint64(n);
    }
    return hit100[(int)n];
}

/* ------------------------------------------------------------ */
/*  main function requested by the build system                  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1 */

    /* cache of already generated sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t last_checked = 0;    /* last integer examined */

    if (cache_len < (size_t)n) {
        /* enlarge cache if necessary */
        if (!cache) {
            cache_cap = 128;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        }
        /* grow when needed */
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        uint64_t candidate = last_checked + 1;
        while (cache_len < (size_t)n) {
            if (hits_100_before_1(candidate)) {
                if (cache_len >= cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)candidate;
            }
            ++candidate;
        }
        last_checked = candidate - 1;
    }
    return cache[n - 1];
}
