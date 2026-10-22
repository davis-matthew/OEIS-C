/*
 * A390676  Numbers k such that there exists a composition whose first sums
 *          are the k-th composition in standard order.
 * https://oeis.org/A390676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 816, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A390676.
   a(n) is the nth integer k such that the k‑th composition
   in standard order (graded reverse‑lexicographic) is the
   list of first sums of some composition.
   The routine tests each k by constructing its composition
   and verifying the existence of a solution for the
   predecessor recurrence.  It uses a simple search and
   a linear time test, well within the limits for n≤10000. */

#include <stddef.h>
#include <limits.h>
#include <stdbool.h>

/* check whether the k‑th composition (in standard order)
   can be the first sums of some composition. */
static bool has_solution(idx_t k)
{
    if (k == 0)   /* empty composition works: any single‑part composition */
        return true;

    /* parts of the composition derived from the reversed binary expansion of k.
       parts[i] holds successive interval lengths between consecutive indices
       of 1 bits.  The composition itself is the reverse of this array. */
    int parts[65];      /* 64 bits + possible extra */
    int len = 0;

    idx_t temp = k;
    idx_t prev = 0;          /* previous index+1 (0 initially) */
    idx_t bitidx = 0;

    while (temp) {
        if (temp & 1) {
            parts[len++] = (int)((bitidx + 1) - prev);
            prev = bitidx + 1;
        }
        temp >>= 1;
        ++bitidx;
    }

    /* constraints : there exists integer x such that all
       c_i = sign*x + S are ≥ 1.  Sign alternates +1,-1,... */
    long long L = LLONG_MIN;   /* lower bound for x */
    long long U = LLONG_MAX;   /* upper bound for x */
    int sign = 1;              /* +1 for odd i, -1 for even i */
    long long S = 0;           /* current S in the recurrence */

    /* iterate over composition parts in correct order (reverse of parts array) */
    for (int j = len - 1; j >= 0; --j) {
        int d = parts[j];

        if (sign == 1) {          /* x + S ≥ 1  ⇒  x ≥ 1 - S */
            long long bound = 1LL - S;
            if (bound > L) L = bound;
        } else {                  /* -x + S ≥ 1  ⇒  x ≤ S - 1 */
            long long bound = S - 1LL;
            if (bound < U) U = bound;
        }

        /* update S and sign for next part */
        S = (long long)d - S;
        sign = -sign;
    }

    /* final constraint for c_{n+1} */
    if (sign == 1) {
        long long bound = 1LL - S;
        if (bound > L) L = bound;
    } else {
        long long bound = S - 1LL;
        if (bound < U) U = bound;
    }

    return L <= U;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        if (has_solution(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
