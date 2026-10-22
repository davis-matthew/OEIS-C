/*
 * A328100  Column which is two positions right of the middle column of
 *          rule-30 1-D cellular automaton, when started from a lone 1 cell.
 * https://oeis.org/A328100
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000035(floor(A110240(n) / 2^(n-2)))
 * Range:     u8 n <= 9801, u16 n <= 9801, u32 n <= 9801, u64 n <= 9801,
 *            u128 n <= 9801, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for n < 2, return 0 */

    /* Allocate arrays big enough for the largest needed index.
       We use a fixed offset so that the middle column is always at
       index offset = n.  The arrays are sized to hold indices
       from 0 up to 2*n+2 inclusive. */
    size_t offset = n;
    size_t max_len = 2 * n + 3;      /* 2*n+2 max index + 1 for safety */
    uint8_t *prev = (uint8_t *)calloc(max_len, sizeof(uint8_t));
    uint8_t *next = (uint8_t *)calloc(max_len, sizeof(uint8_t));
    if (!prev || !next) {
        /* Allocation failed – return 0 as a fall‑back */
        free(prev);
        free(next);
        return (seq_t)0;
    }

    /* Initial state: single 1 at the middle */
    prev[offset] = 1;

    /* Iterate up to step n */
    for (size_t step = 1; step <= n; ++step) {
        /* active indices are [offset-step, offset+step] */
        size_t lo = offset > step ? offset - step : 0;
        size_t hi = offset + step;

        for (size_t i = lo; i <= hi; ++i) {
            uint8_t l = (i > 0) ? prev[i - 1] : 0;
            uint8_t c = prev[i];
            uint8_t r = (i + 1 < max_len) ? prev[i + 1] : 0;

            /* Rule 30: next = left XOR (center OR right) */
            next[i] = l ^ (c | r);
        }

        /* Swap pointers: prev will hold the new row */
        uint8_t *tmp = prev;
        prev = next;
        next = tmp;
        /* No need to clear next – past values outside the active
           range are never read again. */
    }

    /* Extract the desired cell: two positions right of the middle */
    seq_t result = (seq_t)prev[offset + 2];

    free(prev);
    free(next);
    return result;
}
