/*
 * A375674  a(n) is the area/perimeter ratio of the rectangle whose edges are
 *          n and A375673(n).
 * https://oeis.org/A375674
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal integer a such that (2*a*n) is divisible by (n -
 *            2*a) and 4*a ≥ n. a(n) is the area/perimeter ratio of the
 *            rectangle with edges n and A375673(n)
 * Range:     u8 n <= 520, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;                       /* offset not needed */
    idx_t start = (n + 3) / 4;                 /* ceil(n/4) – ensures m ≥ n */
    idx_t max_a = (n - 1) / 2;                 /* floor((n-1)/2) – a < n/2 */

    for (idx_t a_val = start; a_val <= max_a; ++a_val)
    {
        idx_t denom = n - 2 * a_val;           /* n – 2a  (positive for a < n/2) */
        idx_t part  = (2 * a_val % denom) * (n % denom);
        if (part % denom == 0)                /* check (2*a*n) % denom == 0 */
            return (seq_t)a_val;               /* this a gives the required ratio */
    }
    /* In a correctly defined sequence a solution exists for all n ≥ 3.
       The following return is never reached.        */
    return 1;
}
