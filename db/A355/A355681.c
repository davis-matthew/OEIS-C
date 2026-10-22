/*
 * A355681  The "coarser" of 2 representations of the Cantor middle thirds set
 *          viewed from a quarter point that lies at a(0) (the third 1 in the
 *          data).
 * https://oeis.org/A355681
 *
 * Offset:    -13    (a(-13) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3280, i16 n <= 3280, i32 n <= 3280, i64 n <= 3280,
 *            i128 n <= 3280, big any n
 * Helpers:   floor_div3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive implementation of A355681.
   The sequence a(n) is defined as f(n) where
     f(0) = 1, and for an integer m and i∈{‑1,0,1}
         f(3m+i) is determined from f(‑m) by the table
             f(‑m)=‑1 → (0, ‑1, 1)
             f(‑m)= 0 → (0,  0, 0)
             f(‑m)= 1 → (‑1, 1, 0)
   This implements that mapping directly.  The function returns
   a(n) for any n ≥ ‑13.  For n < ‑13 the behaviour is unspecified.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_div3.h"

static inline seq_t f_compute(idx_t n)
/* recursive computation of f(n) */
{
    if (n == 0) return (seq_t)1;

    idx_t m = floor_div3(n + 1);        /* m = ⌊(n+1)/3⌋, i ∈ {‑1,0,1} */
    idx_t i = n - 3 * m;                /* i = n – 3m   */

    seq_t k = f_compute(-m);            /* k = f(‑m)    */

    if (k == 0) return 0;

    if (k == -1) { /* mapping for f(‑m)=‑1 → (0,‑1,1) */
        if (i == -1) return 0;
        if (i == 0)  return (seq_t)-1;
        /* i == 1 */ return (seq_t)1;
    } else { /* k == 1, mapping → (‑1,1,0) */
        if (i == -1) return (seq_t)-1;
        if (i == 0)  return (seq_t)1;
        /* i == 1 */ return 0;
    }
}

seq_t a(idx_t n)
/* public entry‑point for the sequence */
{
    return f_compute(n);
}
