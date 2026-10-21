/*
 * A257020  Numbers whose quarter-square representation consists of three
 *          terms.
 * https://oeis.org/A257020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_quarter_squares, index_of_max_leq, three_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: for each integer m, repeatedly subtract the largest
 * quarter square ≤ the remainder (the greedy algorithm).
 * If the subtraction terminates with exactly three distinct terms,
 * m is stored.  The function returns the nth such integer (1‑based).
 * This reproduces OEIS A257020.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_quarter_squares.h"
#include "../utils/index_of_max_leq.h"
#include "../utils/three_terms.h"

/*  main sequence function                                                    */
seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *quarter_squares = NULL;
    static size_t quarter_cnt = 0;
    static seq_t *values = NULL;
    static size_t val_len = 0, val_cap = 0;
    static seq_t next_m = 1;          /* next integer to test */

    /* ensure quarter squares exist */
    if (!quarter_squares) {
        const seq_t LIMIT = (seq_t)100000000; /* 1e8 – ample for 10kth term */
        build_quarter_squares(&quarter_squares, &quarter_cnt, LIMIT);
    }

    /* produce values until we have nth */
    while (val_len < (size_t)n) {
        /* extend storage if necessary */
        if (val_len >= val_cap) {
            size_t new_cap = val_cap == 0 ? 128 : val_cap << 1;
            values = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
            val_cap = new_cap;
        }
        if (three_terms(quarter_squares, quarter_cnt, next_m))
            values[val_len++] = next_m;
        ++next_m;
    }
    return values[n - 1];
}
