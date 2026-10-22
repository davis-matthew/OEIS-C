/*
 * A356895  a(n) is the length of the maximal tribonacci representation of n
 *          (A352103).
 * https://oeis.org/A356895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest k≥1 such that n ≤ Σ_{i=1..k} T'_i , T'_i :
 *            the i‑th distinct positive Tribonacci number (i.e.
 *            1,2,4,7,13,24,…) . For n=0 the function returns 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;      /* a(0) = 1 by definition */

    /* Generate distinct positive Tribonacci numbers until their cumulative sum
       reaches or exceeds n.  All arithmetic is done in idx_t to avoid truncation. */
    idx_t sum   = 0;          /* running cumulative sum of distinct positive Tribonacci numbers */
    idx_t last  = 0;          /* last distinct positive Tribonacci number added to sum */
    idx_t count = 0;          /* number of distinct positive Tribonacci numbers added (k) */

    /* Tribonacci recurrence: a0 = 0, a1 = 0, a2 = 1, ai = ai-3 + ai-2 + ai-1 */
    idx_t t0 = 0, t1 = 0, t2 = 1;   /* a0, a1, a2 */

    /* First distinct positive Tribonacci number is '1' */
    sum   += t2;
    last  = t2;
    count = 1;

    /* Generate further terms until sum ≥ n */
    while (sum < n) {
        /* Next Tribonacci number */
        idx_t t3 = t0 + t1 + t2;

        /* Shift indices */
        t0 = t1;
        t1 = t2;
        t2 = t3;

        /* Consider t2 only if it is a new distinct positive number */
        if (t2 > last && t2 > (idx_t)0) {
            sum  += t2;
            last  = t2;
            ++count;
        }
    }

    return (seq_t)count;       /* count fits into seq_t for the required ranges */
}
