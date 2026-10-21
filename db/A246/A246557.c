/*
 * A246557  Numbers n with an even number of digits such that the product of
 *          the numbers formed by successively deleting pairs of digits from
 *          the ends of n is equal to n.
 * https://oeis.org/A246557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute force search for all 6‑digit numbers
   that satisfy the property of OEIS A246557 (inner‑substring product equals the number). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t vals_len = 0, vals_cap = 0;
    static int init_done = 0;

    if (!init_done) {
        /* reserve some space for the answers */
        vals_cap = 16;
        vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));
        if (!vals) abort();

        /* enumerate all 6‑digit numbers (first digit non‑zero) */
        for (uint32_t num = 100000; num <= 999999; ++num) {
            uint32_t inner4 = (num / 10) % 10000;   /* digits 2‑5 */
            uint32_t inner2 = (num / 100) % 100;    /* digits 3‑4 */
            seq_t prod = (seq_t)inner4 * (seq_t)inner2;

            if (prod == (seq_t)num) {
                if (vals_len == vals_cap) {
                    vals_cap <<= 1;
                    vals = (seq_t *)realloc(vals, vals_cap * sizeof(seq_t));
                    if (!vals) abort();
                }
                vals[vals_len++] = (seq_t)num;
            }
        }
        init_done = 1;
    }

    if (n == 0 || n > vals_len) return (seq_t)0;
    return vals[n - 1];
}
