/*
 * A241898  a(n) is the largest integer such that n = a(n)^2 + ... is a
 *          decomposition of n into a sum of at most four nondecreasing
 *          squares.
 * https://oeis.org/A241898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) – the largest integer a such that n can be
   expressed as a sum of at most four non‑decreasing squares
   starting with a². */
#include <stdbool.h>
#include <stdint.h>

/* Integer square root for idx_t values.
   Returns floor(sqrt(n)).  */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid == 0) {
            res = 0;
            low = 1;
            continue;
        }
        if (mid > n / mid) {
            if (mid == 0) break;
            high = mid - 1;
        } else {
            res = mid;
            low = mid + 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0 but give 0 */
    idx_t max_root = isqrt_idx(n);

    for (idx_t a_root = max_root; a_root > 0; --a_root) {
        idx_t a2 = a_root * a_root;
        if (a2 > n) continue;

        idx_t rem = n - a2;
        if (rem == 0) return (seq_t)a_root;

        /* 1 additional square */
        idx_t b_root = isqrt_idx(rem);
        if (b_root >= a_root && b_root * b_root == rem)
            return (seq_t)a_root;

        /* 2 additional squares */
        for (idx_t b = a_root; b <= b_root; ++b) {
            idx_t b2 = b * b;
            idx_t rem2 = rem - b2;
            if (rem2 == 0) return (seq_t)a_root;
            idx_t c_root = isqrt_idx(rem2);
            if (c_root >= b && c_root * c_root == rem2)
                return (seq_t)a_root;
        }

        /* 3 additional squares */
        for (idx_t b = a_root; b <= b_root; ++b) {
            idx_t b2 = b * b;
            idx_t rem2 = rem - b2;
            if (rem2 == 0) continue;
            idx_t c_max = isqrt_idx(rem2);
            for (idx_t c = b; c <= c_max; ++c) {
                idx_t c2 = c * c;
                idx_t rem3 = rem2 - c2;
                if (rem3 == 0) return (seq_t)a_root; /* a,b,c */
                idx_t d_root = isqrt_idx(rem3);
                if (d_root >= c && d_root * d_root == rem3)
                    return (seq_t)a_root; /* a,b,c,d */
            }
        }
    }
    return (seq_t)1;  /* fallback – should never reach here */
}
