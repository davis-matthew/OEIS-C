/*
 * A267857  Length of the period of the continued fraction for the square root
 *          of D, the discriminant of indefinite binary quadratic forms. D is
 *          given in A079896.
 * https://oeis.org/A267857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = length of period of the continued fraction for sqrt(D),
 *            where D is the nth positive integer congruent to 0 or 1 mod 4
 *            and not a perfect square (A079896). The function iterates over
 *            such D-values, finds the nth one and computes its period using
 *            the standard continued‑fraction algorithm
 * Range:     u8 n <= 24, u16 n <= 8004, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Compute the length of the periodic part of the continued fraction of
   sqrt(D).  D must be a non‑square integer with D > 0, and the
   algorithm uses only integer arithmetic. */
static seq_t period_len_of_sqrt(seq_t D)
{
    seq_t a0 = floor_sqrt(D);          /* floor(sqrt(D)) */
    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a   = a0;
    seq_t cnt = 0;

    for (;;) {
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;
        cnt++;
        if (a_next == (seq_t)2 * a0)
            return cnt;
        m = m_next;
        d = d_next;
        a = a_next;
    }
}

/* Main entry point required by the OEIS problem statement. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t D_int = 5;          /* first positive integer that can be a discriminant */

    for (;;) {
        /* D must be ≡ 0 or 1 (mod 4) and not a perfect square. */
        if ((D_int % 4 == 0) || (D_int % 4 == 1)) {
            seq_t D = (seq_t)D_int;
            seq_t s  = floor_sqrt(D);
            if (s * s != D) {          /* not a square */
                count++;
                if (count == n) {
                    return period_len_of_sqrt(D);
                }
            }
        }
        D_int++;                       /* next candidate */
    }
}
