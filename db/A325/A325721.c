/*
 * A325721  Start the sequence with a(1) = 1 and read the digits one by one
 *          from there. The sequence is always extended with the sum d + k, d
 *          being the digit read and k the number of d digits present so far
 *          in the sequence.
 * https://oeis.org/A325721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum (d + k) where d is the digit read and k is the number
 *            of occurrences of d in the sequence so far. The implementation
 *            follows the definition directly
 * Range:     u8 n <= 1318, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_to_str.h"

static seq_t *cache = NULL;        /* cached sequence values */
static idx_t cache_size = 0;       /* number of terms already computed */
static idx_t cache_cap = 0;        /* allocated capacity of cache */

static unsigned int digit_count[10] = {0};   /* occurrences of each digit */
static idx_t read_term_idx = 0;             /* term currently being read */
static size_t read_pos = 0;                 /* position inside current term */
static idx_t string_term_idx = SIZE_MAX;    /* term whose string is in buffer */
static char string_buf[128];                /* decimal string of current term */
static size_t string_len = 0;

/* Ensure cache has capacity for n terms */
static void reserve_cache(idx_t n)
{
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize everything for the first call */
static void init_sequence(void)
{
    reserve_cache(16);
    cache[0] = (seq_t)1;        /* a(1) */
    cache_size = 1;
    for (int i = 0; i < 10; ++i) digit_count[i] = 0;

    read_term_idx = 0;
    read_pos = 0;
    string_term_idx = SIZE_MAX; /* force recomputation on first access */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1, n>=1 in practice */

    /* Lazy initialization */
    if (cache_size == 0) init_sequence();

    /* Grow the sequence until we have n terms */
    while (cache_size < n) {
        /* Load current term's string if this is a new term */
        if (read_term_idx != string_term_idx) {
            /* get decimal representation of the current term */
            size_t tmp_len;
            seq_to_str(cache[read_term_idx], string_buf, &tmp_len);
            string_len = tmp_len;
            read_pos = 0;
            string_term_idx = read_term_idx;
        }

        /* If we have exhausted the current term, move to the next one */
        if (read_pos >= string_len) {
            ++read_term_idx;
            string_term_idx = SIZE_MAX; /* reset to force reload */
            continue;
        }

        /* Extract the next digit */
        unsigned int d = (unsigned int)(string_buf[read_pos] - '0');
        ++read_pos;

        /* Update count for this digit and generate the next term */
        ++digit_count[d];
        seq_t next_val = (seq_t)d + (seq_t)digit_count[d];

        /* Store the new term */
        reserve_cache(cache_size + 1);
        cache[cache_size++] = next_val;
    }

    return cache[n - 1];
}
