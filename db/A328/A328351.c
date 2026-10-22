/*
 * A328351  Let S be any integer in the range 10 <= S <= 30. Sequence has the
 *          property that a(n)*S is the sum of all positive integers whose
 *          decimal expansion has <= n digits and contains at most four
 *          distinct nonzero digits d1, d2, d3 and d4 such that d1+d2+d3+d4=S.
 * https://oeis.org/A328351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(0)=0, a(1)=1, a(2)=45
   a(n) = 45*a(n-1) - 204*a(n-2) + 160*a(n-3)   for n>2
   (equivalently a(n) = (30*40^n-39*4^n+9)/1053) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 0ULL) return (seq_t)0;
    if (n == 1ULL) return (seq_t)1;
    if (n == 2ULL) return (seq_t)45;

    /* iterative computation, keeping only the last three terms */
    seq_t a0 = (seq_t)0;      /* a(0) */
    seq_t a1 = (seq_t)1;      /* a(1) */
    seq_t a2 = (seq_t)45;     /* a(2) */
    seq_t ai;

    for (idx_t i = 3ULL; i <= n; ++i) {
        /* compute ai = 45*a2 - 204*a1 + 160*a0
           using (45*a2 + 160*a0) - 204*a1 to avoid underflow */
        ai = ( (seq_t)45 * a2 + (seq_t)160 * a0 ) - (seq_t)204 * a1;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
