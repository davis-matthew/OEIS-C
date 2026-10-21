/*
 * A252113  Number of (n+2) X (7+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum equal to 0 2 3 6 or 7 and every 3 X 3 diagonal and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A252113: number of (n+2)×(7+2) 0..3 arrays with the stated
   row/column and diagonal restrictions.
   Empirical recurrence:  a(n) = 8 * a(n-3)  for n > 8.
   Initial values (1‑indexed):
   a(1)=2468, a(2)=1180, a(3)=2476, a(4)=4488,
   a(5)=11760, a(6)=17772, a(7)=33216, a(8)=84864. */

#include <stddef.h>      /* for size_t */

static seq_t pow8(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 8;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Base values for n = 1 .. 8 (0‑th index unused).  Constants are
       expressed as seq_t to avoid accidental type promotion. */
    static const seq_t base[9] = {
        0,          /* unused */
        (seq_t)2468,  /* a(1) */
        (seq_t)1180,  /* a(2) */
        (seq_t)2476,  /* a(3) */
        (seq_t)4488,  /* a(4) */
        (seq_t)11760, /* a(5) */
        (seq_t)17772, /* a(6) */
        (seq_t)33216, /* a(7) */
        (seq_t)84864  /* a(8) */
    };

    if (n <= 8) {
        return base[(size_t)n];
    }

    /* For n > 8, reduce to a value in 1..8 by repeatedly subtracting 3.
       The residue r = n mod 3 determines which base entry to use. */
    idx_t r = n % 3;       /* 0, 1, or 2 */
    idx_t b = 6 + r;       /* base index (6, 7, or 8) */
    seq_t base_val = base[(size_t)b];
    idx_t exp = (n - b) / 3;   /* number of times to multiply by 8 */
    return base_val * pow8(exp);
}
