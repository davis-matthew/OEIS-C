/*
 * A296865  Numbers whose base-4 digits d(m), d(m-1), ..., d(0) have #(pits) >
 *          #(peaks); see Comments.
 * https://oeis.org/A296865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9274 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool pits_over_peaks(seq_t x)
{
    /* collect base‑4 digits, least significant first */
    uint8_t digits[128];
    size_t len = 0;
    seq_t v = x;
    do {
        digits[len++] = (uint8_t)(v % 4);
        v = v / 4;
    } while (v != 0);

    if (len < 3)            /* no interior digit to be a pit or peak */
        return false;

    size_t pits  = 0;
    size_t peaks = 0;
    for (size_t i = 1; i <= len-2; ++i) {
        uint8_t a = digits[i-1];
        uint8_t b = digits[i];
        uint8_t c = digits[i+1];
        if (a > b && b < c)
            ++pits;
        else if (a < b && b > c)
            ++peaks;
    }
    return pits > peaks;
}

/* Sequence: numbers with more pits than peaks in their base‑4 digits.
   Computed by simple enumeration up to the requested index. */

seq_t a(idx_t n)
{
    /* static state to allow non‑monotonic queries */
    static seq_t *terms      = NULL;   /* found terms */
    static size_t   size       = 0;      /* number stored */
    static size_t   cap        = 0;      /* capacity of terms */
    static seq_t   candidate   = 0;      /* last tested candidate */

    if (n == 0)
        return 0;   /* undefined – guard against accidental zero call */

    /* grow the list until we have at least n terms */
    while (size < (size_t)n) {
        /* find the next number that satisfies the condition */
        for (++candidate; ; ++candidate) {
            if (pits_over_peaks(candidate)) {
                /* allocate or grow the storage */
                if (size == cap) {
                    size_t new_cap = cap ? cap * 2 : 8;
                    seq_t *tmp =
                        mem_resize(seq_t, terms, new_cap);
                    if (!tmp)
                        abort();      /* out of memory */
                    terms = tmp;
                    cap   = new_cap;
                }
                terms[size++] = candidate;
                break;              /* move on to fill next term */
            }
        }
    }

    return terms[n-1];
}
