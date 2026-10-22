/*
 * A330926  a(n) = Sum_{k=1..n} (ceiling(n/k) mod 2).
 * https://oeis.org/A330926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k with ceil(n/k) odd * (equivalently, the
 *            number of k for which floor((n-1)/k) is even). * Computed in
 *            O(√n) by grouping indices with equal floor((n-1)/k)
 * Range:     u8 n <= 816, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset in OEIS is 1, so n>=1.  Guard against accidental n==0. */
    if (n == 0)
        return (seq_t)0;

    idx_t m     = n - 1;            /* use m = n-1: ceil(n/k) = floor(m/k)+1   */
    idx_t limit = m + 1;            /* we need k up to n = m+1                  */
    seq_t count = 0;

    for (idx_t k = 1; k <= limit; /* updated inside */) {
        idx_t v = m / k;           /* = floor(m/k)                           */
        idx_t next;
        if (v == 0) {
            next = limit + 1;      /* all remaining k have v = 0              */
        } else {
            next = m / v + 1;      /* first k where floor(m/k) changes        */
        }
        idx_t end = (next <= limit) ? next - 1 : limit;
        idx_t len = end - k + 1;   /* number of k with the same v            */

        if ((v & 1) == 0) {       /* v even → ceil(n/k) odd                 */
            count += (seq_t)len;
        }
        k = next;                 /* move to next block                      */
    }
    return count;
}
