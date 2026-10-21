/*
 * A267276  Decimal representation of the n-th iteration of the "Rule 118"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal representation of the n‑th iteration of Rule 118
 *            * elementary cellular automaton starting with a single ON cell
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed values, using a simple rule lookup
 * and shifting-and‑adding to obtain the decimal value. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static const unsigned char rule_out[8] = {0,1,1,0,1,1,1,0};

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;      /* a(0)…a(max_n) values          */
    static size_t cache_cap  = 0;         /* current capacity of cache     */
    static unsigned char *cur  = NULL;    /* configuration before step     */
    static unsigned char *next = NULL;    /* buffer for next configuration */
    static size_t cur_cap      = 0;       /* capacity of cur/next arrays   */
    static size_t cur_len      = 0;       /* current width of configuration*/
    static size_t computed_n   = (size_t)-1; /* last n that has been computed */

    /* If we already have n pre‑computed, just return it. */
    if (computed_n != (size_t)-1 && n <= computed_n) {
        return cache[n];
    }

    /* Ensure cache is large enough to hold a(n). */
    if (n >= cache_cap) {
        size_t newCap = n + 1;
        cache = (seq_t*)realloc(cache, newCap * sizeof(seq_t));
        cache_cap = newCap;
    }

    /* Initialise for the first call. */
    if (computed_n == (size_t)-1) {
        cur_cap = 1;
        cur = (unsigned char*)malloc(cur_cap * sizeof(unsigned char));
        next = (unsigned char*)malloc(cur_cap * sizeof(unsigned char));
        cur[0] = 1;          /* single ON cell          */
        cur_len = 1;
        cache[0] = (seq_t)1;
        computed_n = 0;
    }

    /* Compute successive iterations up to n. */
    for (size_t step = computed_n + 1; step <= n; ++step) {
        size_t new_len = cur_len + 2;          /* width grows by 2 each step */

        /* Resize buffer if necessary. */
        if (cur_cap < new_len) {
            cur_cap = new_len;
            next = (unsigned char*)realloc(next, cur_cap * sizeof(unsigned char));
        }

        /* Apply Rule 118 to generate the next configuration. */
        for (size_t j = 0; j < new_len; ++j) {
            unsigned char l = (j >= 2 && (j - 2) < cur_len) ? cur[j - 2] : 0;
            unsigned char c = (j >= 1 && (j - 1) < cur_len) ? cur[j - 1] : 0;
            unsigned char r = (j < cur_len) ? cur[j] : 0;
            next[j] = rule_out[(l << 2) | (c << 1) | r];
        }

        /* Convert the binary configuration to its decimal value. */
        seq_t val = 0;
        for (size_t i = 0; i < new_len; ++i) {
            val = (val << 1) + (seq_t)next[i];
        }
        cache[step] = val;

        /* Prepare for the next step. */
        unsigned char *tmp = cur;
        cur = next;
        next = tmp;
        cur_len = new_len;
    }

    computed_n = n;
    return cache[n];
}
