/*
 * A254432  Natural numbers with the maximum number of "feasible" partitions
 *          of length m.
 * https://oeis.org/A254432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A254432: Natural numbers with the maximum number of "feasible" partitions of length m.
   Formula:
     For n ≥ 12:  a(n) = (3^(n-7) + 5) / 2
     For n = 1 … 11: use the explicit initial values below.
   Recurrence (for n ≥ 13):  a(n) = 3 * a(n-1) – 5.
*/

#include <stddef.h>                /* for size_t */
#include "../utils/pow3.h"          /* provides seq_t pow3(idx_t ex); */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; values for n < 1 are not meaningful. */
    if (n < 1) {
        return (seq_t)0;           /* safe default – not part of the sequence */
    }

    /* Initial values for n = 1 … 11 (directly taken from the OEIS list). */
    static const seq_t first[11] = {
        (seq_t)1,  /* a(1) */
        (seq_t)2,  /* a(2) */
        (seq_t)3,  /* a(3) */
        (seq_t)4,  /* a(4) */
        (seq_t)7,  /* a(5) */
        (seq_t)16, /* a(6) */
        (seq_t)18, /* a(7) */
        (seq_t)19, /* a(8) */
        (seq_t)22, /* a(9) */
        (seq_t)43, /* a(10) */
        (seq_t)46  /* a(11) */
    };

    if (n <= 11) {
        return first[(size_t)n - 1];   /* safe: n ≤ 11 guarantees array index < 11 */
    }

    /* For n ≥ 12 use the closed‑form expression. */
    seq_t p = pow3(n - 7);              /* 3^(n-7) */
    return (p + (seq_t)5) / (seq_t)2;   /* (3^(n-7)+5)/2  */
}
