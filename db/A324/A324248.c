/*
 * A324248  Odd numbers with dropping time of the reduced Collatz iteration
 *          (A122458) exceeding 5.
 * https://oeis.org/A324248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A324248: odd numbers a with dropping time of the reduced
   Collatz iteration (A122458) exceeding 5.
   The function returns the n‑th term (n >= 1).
   The dropping time is the first step k in the reduced Collatz
   trajectory of a at which the odd number becomes smaller than a.
   We count only values where that first drop occurs at step >= 6.
   The sequence is sorted, so we check successive odd a in increasing order,
   counting qualifying numbers until we reach the requested index. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Reduced Collatz step: from odd a to the next odd number in the
   3x+1 trajectory. */
static inline seq_t collatz_next(seq_t a)
{
    seq_t x = a * 3 + 1;        /* 3a + 1 is even */
    while ((x & 1) == 0)       /* strip all factors of 2 */
        x >>= 1;
    return x;
}

/* Public interface required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n < 1; guard against misuse */
        return 0;

    idx_t count = 0;          /* number of qualifying odd a found */
    seq_t a_val = 3;          /* start with the first odd > 1 */

    for (;; a_val += 2) {     /* iterate over odd numbers only */
        /* 1 is excluded because its trajectory never drops below itself. */
        if (a_val == 1) continue;

        seq_t cur = a_val;
        idx_t step = 0;

        /* Iterate the reduced Collatz sequence until a smaller odd appears. */
        for (;;) {
            cur = collatz_next(cur);
            ++step;
            if (cur < a_val) {          /* first drop found */
                if (step >= 6) {        /* dropping time > 5 */
                    ++count;
                    if (count == n)
                        return a_val;
                }
                break;                   /* move to the next odd a */
            }

            /* If no drop occurs within a reasonable bound, assume it never
               drops and skip this a to avoid an infinite loop.  For the
               numbers that appear in the OEIS b-file, this bound is far
               above the actual dropping times. */
            if (step > 20000)
                break;
        }
    }
}
