/*
 * A265034  Weak Goodstein sequence beginning with 266.
 * https://oeis.org/A265034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 266; for n >= 1, let b = n+2, write a(n‑1) in base b,
 *            interpret that digit string in base b+1, then subtract 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 13, u64 n <= 253, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)266;

    seq_t current = (seq_t)266;

    /* dynamic array for digits */
    seq_t *digits = NULL;
    size_t cap = 64;
    digits = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!digits) return (seq_t)0;  /* fallback; unlikely to happen */

    for (idx_t step = 0; step < n; ++step) {
        if (current == (seq_t)0) {          /* sequence has terminated */
            break;
        }

        idx_t base_idx = step + 2;          /* current base: b = n+2   */
        seq_t base = (seq_t)base_idx;
        seq_t base_plus1 = base + (seq_t)1;

        /* extract digits of current in base 'base' (least significant first) */
        size_t len = 0;
        while (current != (seq_t)0) {
            if (len == cap) {               /* grow array */
                cap <<= 1;
                digits = (seq_t *)realloc(digits, cap * sizeof(seq_t));
                if (!digits) return (seq_t)0;  /* fallback */
            }
            digits[len++] = current % base;
            current /= base;
        }
        if (len == 0) {                     /* current was zero */
            len = 1;
            digits[0] = (seq_t)0;
        }

        /* interpret the digits in base 'base_plus1' */
        seq_t new_val = (seq_t)0;
        for (long i = (long)len - 1; i >= 0; --i) {
            new_val = new_val * base_plus1 + digits[i];
        }
        new_val = new_val - (seq_t)1;        /* subtract 1 */

        current = new_val;
    }

    free(digits);
    return current;
}
