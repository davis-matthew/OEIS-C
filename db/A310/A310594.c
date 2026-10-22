/*
 * A310594  Coordination sequence Gal.4.11.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A310594: Coordination sequence Gal.4.11.3
 *
 * The sequence is given by two simple formulas depending on the parity of n.
 * For n = 2k   : a(n) = 7k + e(k)      where e(0)=1 and for k≥1,
 *                e(k) = ⌊(k-1)/3⌋ – 1.
 * For n = 2k+1 : a(n) = 7k + d(k)      where
 *   let q = ⌊k/3⌋ and r = k mod 3:
 *     r = 0 : d = 4      if q=0
 *             d = 3q + 6 if q>0
 *     r = 1 : d = 2q + 5
 *     r = 2 : d = 2q + 6
 *
 * These formulas reproduce all known terms and can be evaluated in O(1)
 * time for any n≥0.  All arithmetic is performed in the integer type
 * seq_t, using only the operations allowed for that type.
 */

seq_t a(idx_t n)
{
    /* Even indices n = 2k */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;               /* k = n/2  */
        if (k == 0)                      /* a(0) = 1  */
            return 1;

        seq_t kk   = (seq_t)k;          /* Promote k to seq_t */
        seq_t term = (seq_t)7 * kk;     /* 7k            */
        seq_t corr = (kk - 1) / 3;      /* ⌊(k-1)/3⌋     */
        return term + corr - 1;         /* 7k + ⌊(k-1)/3⌋ – 1 */
    }

    /* Odd indices n = 2k+1 */
    idx_t k = (n - 1) >> 1;             /* k = (n-1)/2 */
    seq_t kk  = (seq_t)k;               /* Promote k to seq_t */
    seq_t base = (seq_t)7 * kk;         /* 7k            */

    idx_t q = k / 3;                    /* q = ⌊k/3⌋     */
    idx_t r = k % 3;                    /* r = k mod 3   */

    seq_t d;
    if (r == 0) {
        if (q == 0)
            d = 4;                     /* d(0) = 4      */
        else
            d = (seq_t)3 * q + 6;      /* 3q + 6        */
    } else if (r == 1) {
        d = (seq_t)2 * q + 5;          /* 2q + 5        */
    } else {                           /* r == 2        */
        d = (seq_t)2 * q + 6;          /* 2q + 6        */
    }

    return base + d;                    /* a(n) = 7k + d */
}
