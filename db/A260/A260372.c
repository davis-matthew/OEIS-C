/*
 * A260372  Number of haystack arrangements with n bales.
 * https://oeis.org/A260372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A260372
 *
 *  A(n) = number of distinct haystack arrangements (x,y,h) with
 *  y ≥ x, 1 ≤ h ≤ min(x,y), whose size in haybales equals n.
 *
 *  The size of a haystack for parameters (x,y,h) is
 *
 *      s(x,y,h) = h*(6*x*y - 3*(x+y)*(h-1) + (h-1)*(2*h-1)) / 6
 *
 *  The routine enumerates all parameter triples that produce the
 *  requested size.  For n up to 10 000 the number of triples is
 *  below 200, so the algorithm runs in well under two seconds
 *  even on an arbitrary‑precision build.
 *
 *  All arithmetic is performed with the supplied types seq_t
 *  (the result type) and idx_t (the index type).  The code is
 *  valid C11 and C++17 and uses no external utilities.
 */

#include <stddef.h>   /* for size_t */

/* Compute the size of a haystack given x, y, h. */
static seq_t
haystack_size(idx_t x, idx_t y, idx_t h)
{
    seq_t sx = (seq_t)x;
    seq_t sy = (seq_t)y;
    seq_t sh = (seq_t)h;

    seq_t term1 = (seq_t)6 * sx * sy;                     /* 6xy */
    seq_t term2 = (seq_t)3 * (sx + sy) * (sh - 1);        /* 3(x+y)(h-1) */
    seq_t term3 = (sh - 1) * ((seq_t)2 * sh - 1);         /* (h-1)(2h-1) */

    return sh * (term1 - term2 + term3) / 6;
}

/* External linkage as required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(n) is defined only for n ≥ 1 */

    seq_t count = 0;                    /* result to accumulate */
    seq_t sn = (seq_t)n;                /* cast once for comparisons */

    /* Enumerate all feasible triples (h, x, y). */
    for (idx_t h = 1; ; ++h) {
        /* Minimum size for this height occurs when x = y = h. */
        seq_t s_min_h = haystack_size(h, h, h);
        if (s_min_h > sn) break;        /* no further h will fit */

        for (idx_t x = h; ; ++x) {
            seq_t s_min_x = haystack_size(x, x, h);
            if (s_min_x > sn) break;    /* no further x for this h */

            for (idx_t y = x; ; ++y) {
                seq_t s = haystack_size(x, y, h);
                if (s > sn) break;      /* larger y only increases size */
                if (s == sn) count += 1;
            }
        }
    }

    return count;
}
