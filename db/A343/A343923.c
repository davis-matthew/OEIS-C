/*
 * A343923  If n = Product (p_j^k_j) then a(n) = Sum (abs(p_j-k_j)) (a(1) = 0
 *          by convention).
 * https://oeis.org/A343923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p^k || n} |p - k|
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 */

    uint64_t tmp = n;
    seq_t sum   = 0;

    /* Handle factor 2 separately for speed */
    if ((tmp & 1ULL) == 0) {
        uint64_t cnt = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            cnt += 1;
        }
        seq_t diff = (cnt > 2) ? (seq_t)(cnt - 2) : (seq_t)(2 - cnt);
        sum += diff;
    }

    /* Odd prime factors */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            uint64_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                cnt += 1;
            }
            seq_t diff = (cnt > p) ? (seq_t)(cnt - p) : (seq_t)(p - cnt);
            sum += diff;
        }
    }

    /* Remaining prime factor (if any) */
    if (tmp > 1) {
        uint64_t p   = tmp;
        uint64_t cnt = 1;
        seq_t diff = (cnt > p) ? (seq_t)(cnt - p) : (seq_t)(p - cnt);
        sum += diff;
    }

    return sum;
}
