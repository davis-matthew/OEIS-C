/*
 * A353440  Integers m such that the decimal expansion of 1/m contains the
 *          digit 4.
 * https://oeis.org/A353440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer m such that the decimal
 *            expansion of 1/m contains the digit 4
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/append_candidate.h"

/* ------------------------------------------------------------------
 * Helper: does the decimal expansion of 1/m contain a digit '4' ?
 * ------------------------------------------------------------------ */
static uint64_t *mark = NULL;      /* timestamp for each remainder   */
static size_t   mark_cap = 0;      /* allocated size of mark array   */
static uint64_t cur_tick = 1;      /* next timestamp to use          */

static bool contains_digit4(idx_t m)
{
    if (m <= 1) return false;       /* 1/1 = 1 has no 4 */

    /* Grow the timestamp array if needed. */
    if ((size_t)m > mark_cap) {
        size_t new_cap = ((size_t)m + 1) * 2;
        uint64_t *tmp = (uint64_t *)realloc(mark, new_cap * sizeof(uint64_t));
        if (!tmp) return false;    /* memory failure – treat as false */
        mark = tmp;
        mark_cap = new_cap;
    }

    /* Handle timestamp overflow. Reset all entries if we wrap. */
    if (cur_tick == UINT64_MAX) {
        for (size_t i = 0; i < mark_cap; ++i)
            mark[i] = 0;
        cur_tick = 1;
    }
    uint64_t tick = cur_tick++;

    idx_t r = 1;   /* initial remainder for 1/m */
    while (1) {
        /* 128‑bit arithmetic to compute r*10 / m safely. */
        unsigned __int128 tmp = (unsigned __int128)r * 10;
        idx_t digit = (idx_t)(tmp / m);
        r = (idx_t)(tmp % m);

        if (digit == 4) return true;        /* found a 4 */

        if (r == 0) return false;           /* terminating decimal – no 4 */

        if (mark[r] == tick) return false;  /* cycle detected */
        mark[r] = tick;                     /* remember this remainder */
    }
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */
static seq_t *terms   = NULL;  /* cached terms           */
static size_t terms_len = 0;   /* number of cached terms */
static size_t terms_cap = 0;   /* capacity of array      */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined, but safe */

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        /* test next integer for the property */
        static idx_t next = 1;      /* next candidate to test */
        ++next;
        if (contains_digit4(next))
            append_candidate(&terms, &terms_len, &terms_cap, (seq_t)next);
    }

    return terms[n - 1];
}
