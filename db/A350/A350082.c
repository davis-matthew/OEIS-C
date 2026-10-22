/*
 * A350082  Smallest odd number > n with n in its Collatz successors, or 0 if
 *          no such odd number exists. a(1) = 1.
 * https://oeis.org/A350082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 7072, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function implements OEIS entry A350082:   a(n) is the smallest
 *  odd integer > n that has n as a successor in its Collatz trajectory,
 *  or 0 if no such odd number exists.  The definition guarantees that
 *  0 occurs exactly for n ≡ 0 (mod 3); for all other n an odd predecessor
 *  greater than n exists.  The algorithm simply scans upward through
 *  odd numbers, testing each one by running its Collatz forward
 *  trajectory until it either reaches n (success) or the standard
 *  1‑termination (failure).  Because the existence is guaranteed for
 *  n ≠ 0 (mod 3), the search always terminates quickly for n ≤ 10⁴.  */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 1) return 1;            /* a(1) = 1 */
    if (n % 3 == 0) return (seq_t)0; /* multiples of 3 have no odd predecessor */

    seq_t target = (seq_t)n;          /* value we want to hit */

    /* Start searching from the smallest odd number greater than n */
    seq_t cand = (seq_t)n + 1;
    if ((cand & 1) == 0) cand++;      /* make it odd */

    for (;; cand += 2) {              /* only odd candidates */
        seq_t x = cand;

        /* Follow the Collatz trajectory of x until it either hits the
         * target or reaches 1 (the usual termination).  The standard
         * Collatz conjecture holds for numbers in the range of interest,
         * so this loop will always terminate. */
        while (1) {
            if (x == target) return cand;   /* found the desired predecessor */
            if (x == (seq_t)1) break;       /* no hit, try next odd candidate */

            if ((x & 1) == 0) {
                x = x / 2;
            } else {
                x = x * 3 + 1;
            }
        }
    }
}
