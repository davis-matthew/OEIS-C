/*
 * A342262  Numbers divisible both by the product of their nonzero digits
 *          (A055471) and by the sum of their digits (A005349).
 * https://oeis.org/A342262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 760, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_prod_nonzero_loop, digit_sum_loop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A342262: Numbers divisible by the product of their nonzero digits
   and by the sum of their digits (Niven numbers with the extra
   product divisibility condition).  The function enumerates candidates
   starting from 1 and returns the n-th such number. */

#include <stdbool.h>
#include "../utils/digit_prod_nonzero_loop.h"
#include "../utils/digit_sum_loop.h"

seq_t a(idx_t n)
/* Sequence A342262 */
{
    seq_t m = (seq_t)1;
    idx_t count = 0;
    for (;;) {
        seq_t prod = digit_prod_nonzero_loop(m);
        seq_t sum  = digit_sum_loop(m);
        if (m % prod == (seq_t)0 && m % sum == (seq_t)0) {
            if (++count == n)
                return m;
        }
        m++;
    }
}
