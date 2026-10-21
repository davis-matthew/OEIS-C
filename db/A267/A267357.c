/*
 * A267357  Decimal representation of the n-th iteration of the "Rule 124"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule 124 elementary cellular automaton
   Digital representation of the n‑th state with a single ON cell
   as a base‑2 integer (most significant bit first). */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    /* static cache of already computed a(k) for k < cache_len */
    static seq_t *cache       = NULL;
    static idx_t  cache_cap   = 0;
    static idx_t  cache_len   = 0;

    /* static buffer holding the current binary pattern */
    static unsigned char *bits    = NULL;
    static size_t        bits_len = 0;

    /* Rule 124 truth table: {0,0,1,1,1,1,1,0} for indices 0..7
       (000 → 0, 001 → 0, 010 → 1, 011 → 1, 100 → 1, 101 → 1,
       110 → 1, 111 → 0). */
    static const unsigned char rule124[8] = {0,0,1,1,1,1,1,0};

    /* initialise for n = 0 only once */
    if (cache_len == 0) {
        /* pattern for n = 0 is just a single 1 */
        bits_len = 1;
        bits = (unsigned char *)malloc(bits_len);
        bits[0] = 1;

        /* initialise cache with a(0) = 1 */
        cache_cap  = 64;                /* arbitrary starting size */
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]   = (seq_t)1;
        cache_len  = 1;
    }

    /* If requested n has already been computed, return directly. */
    if (n < cache_len)
        return cache[n];

    /* Compute successive terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* The new state extends the current pattern on both sides:
           length = bits_len + 2. */
        size_t new_len_intermediate = bits_len + 2;
        unsigned char *new_bits =
            (unsigned char *)malloc(new_len_intermediate);

        /* Compute outputs for all cells in the new generation. */
        int first_one = -1, last_one = -1;
        for (size_t j = 0; j < new_len_intermediate; ++j) {
            unsigned char L = (j >= 1 && j - 1 < bits_len) ? bits[j - 1] : 0;
            unsigned char C = (j < bits_len)                  ? bits[j]    : 0;
            unsigned char R = (j + 1 < bits_len)              ? bits[j + 1] : 0;
            unsigned char out = rule124[(L << 2) | (C << 1) | R];
            new_bits[j] = out;
            if (out) {
                if (first_one == -1) first_one = (int)j;
                last_one = (int)j;
            }
        }

        /* If no ON cells occurred (unusual for Rule 124), return 0. */
        seq_t term_val = 0;
        size_t trimmed_len = 0;
        if (last_one >= first_one) {
            trimmed_len = (size_t)(last_one - first_one + 1);

            /* Build the integer value of the trimmed pattern. */
            for (size_t j = first_one; j <= (size_t)last_one; ++j)
                term_val = ((seq_t)term_val << 1) | (seq_t)new_bits[j];
        }

        /* Trim the new_bits array to the interval [first_one,last_one]. */
        if (trimmed_len > 0) {
            memmove(new_bits, new_bits + first_one,
                    trimmed_len * sizeof(unsigned char));
            new_bits = (unsigned char *)realloc(new_bits,
                                                trimmed_len * sizeof(unsigned char));
        }
        free(bits);
        bits = new_bits;
        bits_len = trimmed_len;

        /* Ensure cache has room for the new entry. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            cache = (seq_t *)realloc(cache,
                                      new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[i] = term_val;
        ++cache_len;
    }

    return cache[n];
}
