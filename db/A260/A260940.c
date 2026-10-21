/*
 * A260940  a(n) is the smallest index j>n such that g(j)=0 for the sequence g
 *          defined (for indices > n) by g(n+1)=n and g(i) = g(i-1) -
 *          gcd(i,g(i-1)).
 * https://oeis.org/A260940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   
 * Formula: a(n) is the smallest integer j > n such that g(j)=0, where
 * for each n the auxiliary sequence g is defined for indices i > n by
 *    g(n+1) = n
 *    g(i)   = g(i-1) – gcd(i-1, g(i-1))
 * The function below computes a(n) by simulating this recurrence.
 * Since g|< 2n+1 for all n, the loop terminates in O(n) time.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    idx_t idx = n + 1;          /* current index of g, initially n+1   */
    seq_t g = n;                /* g(n+1) = n                           */

    for (;;) {
        seq_t g_next = g - gcd((seq_t)idx, g); /* compute g(idx+1)      */
        ++idx;                                 /* move to next index   */
        if (g_next == 0)                       /* zero reached → return j */
            return (seq_t)idx;
        g = g_next;                            /* continue with new g   */
    }
}
