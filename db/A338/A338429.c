/*
 * A338429  Maximum number of copies of a 1234 permutation pattern in an
 *          alternating (or zig-zag) permutation of length n + 5.
 * https://oeis.org/A338429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = (2*n*(n + 2)*(n + 1)^2)/3 a(2n-1) = (2*n*(n + 1)*(n^2 +
 *            n + 1))/3 where n = (idx for even)/2 or = (idx+1)/2 for odd
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1, this value is unused */
    if ((n & 1) == 0) { /* even index: n = 2*k */
        idx_t k = n / 2;
        seq_t a_val = (seq_t)k;
        seq_t b_val = (seq_t)(k + 2);
        seq_t orig_c = (seq_t)(k + 1);
        seq_t c_val = orig_c;
        /* divide one factor by 3 to avoid overflow */
        if (orig_c % 3 == 0) {
            c_val = orig_c / 3;
        } else if (a_val % 3 == 0) {
            a_val /= 3;
        } else if (b_val % 3 == 0) {
            b_val /= 3;
        }
        seq_t res = 2;
        res *= a_val;
        res *= b_val;
        res *= c_val;
        res *= orig_c; /* the second copy of (k+1) */
        return res;
    } else { /* odd index: n = 2*k - 1 -> k = (n+1)/2 */
        idx_t k = (n + 1) / 2;
        seq_t a_val = (seq_t)k;
        seq_t b_val = (seq_t)(k + 1);
        seq_t c_val = (seq_t)(k * (seq_t)k + k + 1);
        /* divide one factor by 3 */
        if (c_val % 3 == 0) {
            c_val /= 3;
        } else if (a_val % 3 == 0) {
            a_val /= 3;
        } else { /* b_val must be divisible by 3 in this case */
            b_val /= 3;
        }
        seq_t res = 2;
        res *= a_val;
        res *= b_val;
        res *= c_val;
        return res;
    }
}
