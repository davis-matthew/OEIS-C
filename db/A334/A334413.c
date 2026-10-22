/*
 * A334413  First differences of A101803.
 * https://oeis.org/A334413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n+1)/phi) - round(n/phi)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

static idx_t fibs[128];
static size_t fibs_len = 0;

static seq_t recursive_bit(size_t k, idx_t pos) {
    if (k == 0) return 1;
    if (k == 1) return (pos == 0 ? 1 : 0);
    idx_t len_km1 = fibs[k - 1];
    if (pos < len_km1)
        return recursive_bit(k - 1, pos);
    else {
        idx_t pos_rev = (len_km1 + fibs[k - 2]) - 1 - pos;
        return recursive_bit(k - 2, pos_rev);
    }
}

seq_t a(idx_t n) {
    /* Initialize Fibonacci lengths for S(k) */
    if (fibs_len == 0) {
        fibs[0] = 1;
        fibs[1] = 2;
        fibs_len = 2;
    }
    /* Extend until a length larger than n is achieved */
    while (fibs[fibs_len - 1] <= n) {
        fibs[fibs_len] = fibs[fibs_len - 1] + fibs[fibs_len - 2];
        ++fibs_len;
    }
    /* Find the smallest k with fibs[k] > n */
    size_t k = 0;
    while (fibs[k] <= n)
        ++k;
    return recursive_bit(k, n);
}
