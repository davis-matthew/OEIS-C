/*
 * A385616  Indices of pentagonal numbers that are one-fifth of another one.
 * https://oeis.org/A385616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the index m ≥ 0 such that 5·P(m) = P(k) for some k,
 *            where P(t)=t·(3t‑1)/2 is the t‑th pentagonal number. The
 *            sequence starts with a(1)=0, a(2)=1, a(3)=39, a(4)=85705, …
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   next_a, next_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/next_a.h"
#include "../utils/next_b.h"

/* Check that a = 6m-1 and b = 6k-1 are both ≡5 (mod 6) */
static inline bool is_valid(seq_t a, seq_t b)
{
    return (a % 6 == 5) && (b % 6 == 5);
}

/* Advance a branch to its next valid solution and return the corresponding m */
static void advance_branch(seq_t *a, seq_t *b, seq_t *m)
{
    seq_t nb, na;
    do {
        nb = 9*(*b) + 20*(*a);
        na = 4*(*b) + 9*(*a);
        *b = nb;
        *a = na;
    } while (!is_valid(*a, *b));
    *m = (*a + 1)/6;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 0;               /* offset 1: a(1)=0 */

    /* Branch 1 starts from the fundamental solution (b,a)=(1,1)   */
    seq_t a1 = 1, b1 = 1, m1 = 0;
    /* Branch 2 starts from the second fundamental solution (b,a)=(11,5) */
    seq_t a2 = 5, b2 = 11, m2 = 0;

    /* Find the first valid solution in each branch */
    do {
        seq_t nb = 9*b1 + 20*a1;
        seq_t na = 4*b1 + 9*a1;
        b1 = nb; a1 = na;
    } while (!is_valid(a1, b1));
    m1 = (a1 + 1)/6;

    /* Branch 2 is already valid */
    m2 = (a2 + 1)/6;

    idx_t count = 1;   /* a(1)=0 already counted */

    while (1) {
        seq_t next_val;
        if (m1 <= m2) {
            next_val = m1;
            ++count;
            if (count == n) return next_val;
            /* advance branch 1 to its next valid solution */
            do {
                seq_t nb = 9*b1 + 20*a1;
                seq_t na = 4*b1 + 9*a1;
                b1 = nb; a1 = na;
            } while (!is_valid(a1, b1));
            m1 = (a1 + 1)/6;
        } else {
            next_val = m2;
            ++count;
            if (count == n) return next_val;
            /* advance branch 2 to its next valid solution */
            do {
                seq_t nb = 9*b2 + 20*a2;
                seq_t na = 4*b2 + 9*a2;
                b2 = nb; a2 = na;
            } while (!is_valid(a2, b2));
            m2 = (a2 + 1)/6;
        }
    }
}
