/*
 * A260451  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (2,3,1) and midword sequence (a(n)); see Comments.
 * https://oeis.org/A260451
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

/*
 * Recursive definition of the infinite palindromic word A260451
 * a(1)=2, a(2)=3, a(3)=1.
 * For n>3:
 *   Let t = ⌊log2 n⌋.
 *   If n is a power of two, a(n) = a(t-1).
 *   Otherwise a(n) = a(2^{t+1} - n).
 * This implements the construction w(k+1)=w(k)+a(k)+w(k)*.
 */

#include <stdint.h>

/* compute ⌊log2 v⌋ for v > 0 */
static inline idx_t floor_log2(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) {
        ++r;
    }
    return r; /* v is positive */
}

/* main function required by the problem */
seq_t a(idx_t n)
{
    static const seq_t base[3] = {(seq_t)2, (seq_t)3, (seq_t)1};

    if (n <= 3) {
        return base[n - 1];
    }

    /* power of two case */
    if ((n & (n - 1)) == 0) {
        /* n = 2^t, t >= 1 */
        idx_t t = floor_log2(n);
        /* for n == 2 the recursion would call a(1) -> handled below */
        return a(t - 1);
    }

    /* general case: reflect across the surrounding palindrome */
    idx_t t   = floor_log2(n);           /* 2^t <= n < 2^{t+1} */
    idx_t next = ((idx_t)1) << (t + 1);  /* 2^{t+1} */
    idx_t m = next - n;                  /* mirrored index, 1 <= m < 2^t */

    return a(m);
}
