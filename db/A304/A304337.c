/*
 * A304337  Lexicographically earliest fractal-like sequence such that the
 *          erasure of all pairs of contiguous terms of opposite parity leaves
 *          the sequence unchanged.
 * https://oeis.org/A304337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Let T_k = k(k+1)/2. If n == T_k -> a(n) = 2k-1 (k odd) or 2k (k
 *            even). If n == T_k + 1 -> a(n) = 2k (k odd) or 2k-1 (k even).
 *            Otherwise (free term) -> a(n) = a(n-2k). This recurrence
 *            generates OEIS A304337
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>

/* Helper: find greatest k such that T_k <= n (T_k = k(k+1)/2).          */
static inline idx_t find_k(idx_t n)
{
    idx_t low = 0, high = 1;
    /* first find an upper bound for k  */
    while (((unsigned __int128)high * (high + 1)) / 2 <= n) {
        high <<= 1;
    }
    /* binary search in [low, high)  */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    assert(n >= 1);            /* the sequence starts at index 1 */
    while (1) {
        idx_t k = find_k(n);
        unsigned __int128 Tk128 = (unsigned __int128)k * (k + 1) / 2;
        idx_t Tk = (idx_t)Tk128;
        if (n == Tk) {                 /* pair start */
            if (k & 1)                 /* k odd   */
                return (seq_t)(2 * k - 1);
            else                        /* k even  */
                return (seq_t)(2 * k);
        } else if (n == Tk + 1) {       /* pair end   */
            if (k & 1)                 /* k odd   */
                return (seq_t)(2 * k);
            else                        /* k even  */
                return (seq_t)(2 * k - 1);
        } else {                        /* free term */
            n -= 2 * k;                 /* recurse to earlier index   */
        }
    }
}
