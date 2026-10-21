/*
 * A260445  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (0,0,1) and midword sequence (a(n)); see Comments.
 * https://oeis.org/A260445
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

/* Recursively compute the term a(n) of A260445.  
 * Base cases: a(1)=0, a(2)=0, a(3)=1.
 * For n > 3:
 *   Let k = ⌊log₂ n⌋  (largest e with 2^e ≤ n) and mid = 2^k.
 *   • If n == mid   →  a(n) = a(k‑1)        (middle of a palindrome step)
 *   • If n > mid    →  a(n) = a(2^(k+1) – n) (mirror position on the right side)
 *   • If n < mid    →  remain in the left half (loop continues with same n).
 * This loop reduces n to one of {1,2,3} in O(log n) time. */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 1;

    /* Reduce n until it reaches the base indices */
    while (n > 3) {
        /* Check if n is a power of two: n = 2^k */
        if ((n & (n - 1)) == 0) {
            /* n = 2^k  →  a(n) = a(k-1) */
            idx_t k = 0;
            idx_t t = n;
            while (t >>= 1) ++k;          /* k = floor(log₂ n) */
            if (k <= 1) return 0;         /* safety for n==2 (handled earlier) */
            n = k - 1;                    /* recurse on smaller index */
            continue;
        }

        /* Find k with 2^k ≤ n < 2^(k+1) */
        idx_t k = 0;
        idx_t t = n;
        while (t >>= 1) ++k;              /* k = floor(log₂ n) */
        idx_t mid = ((idx_t)1) << k;      /* mid = 2^k */

        if (n > mid) {
            /* Right side of the palindrome: mirror to the left side */
            idx_t len = ((idx_t)1 << (k + 1)) - 1;  /* L(k) = 2^(k+1) – 1 */
            n = len - n + 1;                       /* mirrored position */
        }
        /* If n < mid, we are on the left side – continue the loop unchanged */
    }

    /* n is now 1, 2, or 3 */
    if (n == 1) return 0;
    if (n == 2) return 0;
    return 1;   /* n == 3 */
}
