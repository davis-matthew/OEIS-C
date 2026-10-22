/*
 * A372324  Expansion of e.g.f. arcsin(x)^2/(2*(1 - x)).
 * https://oeis.org/A372324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A372324
 * Expansion of e.g.f. arcsin(x)^2/(2*(1 - x)).
 *
 * Recurrence:
 *   a(0) = 0, a(1) = 0
 *   a(2*n+1) = (2*n+1) * a(2*n)
 *   a(2*n)   = (2*n)*(2*n-1) * a(2*n-2) + ((2*n-2)!!)^2
 *
 * Implementation:  the recurrence is evaluated iteratively.  Only the
 * last even‑indexed term and a running double‑factorial value are kept.
 * All arithmetic is performed in the type seq_t, which is
 * either a fixed‐width unsigned integer or an arbitrary‑precision
 * integer provided by the build.  The routine runs in O(n) time and
 * O(1) extra memory, and works for any order of calls. */

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;
    }

    /* a[0] and a[1] are 0 */
    seq_t prev_even   = (seq_t)0;   /* a[i-2] for the current even i   */
    seq_t last_even   = (seq_t)0;   /* a[i-1] when i is odd (even last) */
    seq_t df          = (seq_t)1;   /* (i-2)!! for the current even i  */
    seq_t term        = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1ULL) == 0ULL) {   /* even index */
            term = (seq_t)i * (seq_t)(i - 1) * prev_even + df * df;
            /* after computing a[i], it becomes the new previous even
               for the next even step and the last even for an odd step */
            prev_even = term;
            last_even = term;
            df = df * (seq_t)i;     /* update (i-2)!! to (i)!! for next loop */
        } else {                    /* odd index */
            term = (seq_t)i * last_even;
        }

        if (i == n) {
            return term;
        }
    }

    /* n == 0 or 1 handled above; unreachable otherwise */
    return (seq_t)0;
}
