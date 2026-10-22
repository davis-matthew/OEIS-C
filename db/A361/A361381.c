/*
 * A361381  In continued fraction convergents of sqrt(d), where d=A005117(n)
 *          (squarefree numbers), the position of a/b where abs(a^2 - d*b^2) =
 *          1 or 4.
 * https://oeis.org/A361381
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 77, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   floor_sqrt, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes A361381.
 *  For n ≥ 5 let d = A005117(n) (the n‑th squarefree number).
 *  Compute the continued‑fraction convergents of √d and return the
 *  1‑based position of the first convergent a/b that satisfies
 *  |a² – d·b²| = 1 or 4.  (The first convergent is at position 1.)
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/floor_sqrt.h"

/* Return the n-th squarefree number (n ≥ 1). */
static seq_t get_squarefree(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1;; ++x) {
        if (is_squarefree((seq_t)x)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 5)          /* offset is 5; behavior below 5 is irrelevant */
        return 0;

    /* d = A005117(n) */
    seq_t d = get_squarefree(n);

    /* a0 = floor(√d) */
    seq_t a0 = floor_sqrt(d);

    /* Variables used in the continued‑fraction algorithm */
    seq_t m0     = 0;     /* m₀ */
    seq_t d0     = 1;     /* d₀ */
    seq_t ak     = a0;    /* a₀ then a₁, a₂, … */

    /* Convergent recurrence:  p₋₂=0, p₋₁=1;  q₋₂=1, q₋₁=0 */
    seq_t p_prevprev = 0, p_prev = 1;
    seq_t q_prevprev = 1, q_prev = 0;

    for (idx_t k = 0;; ++k) {
        /* Next convergent (k‑th) */
        seq_t p = ak * p_prev + p_prevprev;
        seq_t q = ak * q_prev + q_prevprev;

        /* |p² – d·q²| */
        seq_t lhs = p * p;
        seq_t rhs = d * (q * q);
        seq_t diff = (lhs > rhs) ? (lhs - rhs) : (rhs - lhs);

        if (diff == 1 || diff == 4)
            return (seq_t)(k + 1);   /* position is (k+1) */

        /* Shift previous terms */
        p_prevprev = p_prev;  p_prev = p;
        q_prevprev = q_prev;  q_prev = q;

        /* Generate next partial quotient a_{k+1} */
        seq_t m_next = d0 * ak - m0;
        seq_t d_next = (d - m_next * m_next) / d0;
        seq_t a_next = (a0 + m_next) / d_next;

        m0 = m_next;
        d0 = d_next;
        ak = a_next;
    }
}
