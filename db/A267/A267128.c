/*
 * A267128  Decimal representation of the n-th iteration of the "Rule 99"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 4 a(2) = 10 a(3) = 105 a(4) = 100 a(5) = 1875
 *            a(6) = 840 a(7) = 31527 a(8) = 6800 a(9) = 514639 a(10) = 55584
 *            For n ≥ 11 the sequence satisfies the linear recurrence a(n) =
 *            27*a(n-2) + 8*a(n-3) - 202*a(n-4) - 216*a(n-5) + 432*a(n-6) +
 *            1616*a(n-7) - 256*a(n-8) - 3456*a(n-9) + 2048*...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[11] = {
    1ULL,  /* a(0) */
    4ULL,  /* a(1) */
    10ULL, /* a(2) */
    105ULL,/* a(3) */
    100ULL,/* a(4) */
    1875ULL,/* a(5) */
    840ULL,/* a(6) */
    31527ULL,/* a(7) */
    6800ULL,/* a(8) */
    514639ULL,/* a(9) */
    55584ULL /* a(10) */
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;   /* number of terms stored */

    /* Grow and initialise cache if it doesn't exist yet */
    if (!cache) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* Ensure cache has entries up to index n */
    while (len <= (size_t)n) {
        if (len >= cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cap   = newcap;
        }

        if (len < 11) {
            cache[len] = init[len];
        } else {
            size_t i = len;
            seq_t next = cache[i-2] * 27 + cache[i-3] * 8;
            next -= cache[i-4] * 202;
            next -= cache[i-5] * 216;
            next += cache[i-6] * 432;
            next += cache[i-7] * 1616;
            next -= cache[i-8] * 256;
            next -= cache[i-9] * 3456;
            next += cache[i-11] * 2048;
            cache[i] = next;
        }
        len++;
    }

    return cache[(size_t)n];
}
