/*
 * A260452  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (2,1,3) and midword sequence (a(n)); see Comments.
 * https://oeis.org/A260452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260452: Infinite palindromic word
   Construction: let w(1) = (2,1,3). For k ≥ 2,
   w(k) = w(k-1) ⋅ a(k-1) ⋅ reverse(w(k-1)).
   The sequence a(n) is the infinite word obtained as the limit of w(k).

   Recurrence used by the implementation:
     a(1)=2, a(2)=1, a(3)=3.
     For n > 3:
       let p = highest power of 2 with p ≤ n
       if n == p then a(n) = a( log2(p) – 1 )
       else a(n) = a( 2·p – n )   (palindromic symmetry)
   The loop iteratively reduces n until it falls in the base case. */
seq_t a(idx_t n)
{
    /* Base cases 1, 2, 3 */
    if (n == 1) return 2;
    if (n == 2) return 1;
    if (n == 3) return 3;

    /* Reduce n until it is 1, 2, or 3 */
    while (n > 3) {
        /* Find highest power of two ≤ n efficiently */
        idx_t p = 1;
        /* While doubling would stay ≤ n, double p.
           (p > n>>1 guarantees that 2*p would exceed n or overflow.) */
        while (p <= (n >> 1)) p <<= 1;

        if (n == p) {
            /* n is a power of two: use a(n) = a( log2(n) – 1 ) */
            idx_t k = 0;          /* k = floor(log2(p)) */
            idx_t temp = p;
            while (temp >>= 1) ++k;
            n = k - 1;           /* n becomes an index < n */
        } else {
            /* Mirror across the centre: a(n) = a( 2*p – n ) */
            idx_t diff = n - p;   /* n > p ⇒ diff < p */
            n = p - diff;        /* 2*p – n = p - diff */
        }
    }

    /* After reduction, n is 1, 2, or 3 */
    if (n == 1) return 2;
    if (n == 2) return 1;
    return 3; /* n == 3 */
}
