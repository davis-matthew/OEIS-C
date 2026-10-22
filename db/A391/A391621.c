/*
 * A391621  Number of nonnegative integer sequences whose first sums are the
 *          k-th composition in standard order.
 * https://oeis.org/A391621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of non‑negative integer sequences whose *
 *            first‑sums equal the k‑th composition in standard order. * The
 *            implementation maps n to its composition and then counts the *
 *            integer s satisfying the derived linear inequalities
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n) {
    /* find the sum m of the composition */
    idx_t pow = 1;          /* 2^(m-1) */
    idx_t m = 1;
    while (pow <= n/2) {    /* avoid overflow */
        pow <<= 1;
        ++m;
    }
    /* offset within compositions of sum m (0‑based) */
    idx_t offset = n - pow;

    /* decode composition parts from offset bits.
       bits of offset (LSB first) indicate splits after elements. */
    int parts[65];
    int t = 0;              /* number of parts */
    int64_t cur = 1;
    for (idx_t i = 0; i < m-1; ++i) {
        if ((offset >> i) & 1ULL) {  /* split here */
            parts[t++] = (int)cur;
            cur = 1;
        } else {
            ++cur;
        }
    }
    parts[t++] = (int)cur;         /* last part */

    /* compute bounds for the free variable s */
    int64_t lower = 0;             /* s >= 0 automatically */
    int64_t upper = INT64_MAX;     /* large initial upper bound */
    int64_t C = 0;                 /* C_i as defined in analysis */

    for (int i = 1; i <= t + 1; ++i) {
        if (i & 1) {  /* odd i: coefficient of s is +1 */
            if (-C > lower)
                lower = -C;    /* s >= -C */
        } else {      /* even i: coefficient of s is -1 */
            if (C < upper)
                upper = C;   /* s <= C */
        }
        if (i <= t) {
            /* C_{i+1} = p_i - C_i */
            C = (int64_t)parts[i-1] - C;
        }
    }

    if (upper < lower)
        return 0;
    /* count of integer s: upper - lower + 1 */
    return (seq_t)(upper - lower + 1);
}
