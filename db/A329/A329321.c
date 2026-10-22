/*
 * A329321  a(n) is the total number of odd parts in all partitions of n into
 *          consecutive parts.
 * https://oeis.org/A329321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ number of odd parts of each consecutive-part partition
 *            of n = Σ_{k≥1, k(k‑1)/2 < n, (n‑k(k‑1)/2) divisible by k}
 *            oddcount(k, a), where a = (n‑k(k‑1)/2)/k and oddcount(k, a) = {
 *            k/2, if k even { (k+1)/2, if k odd and a odd { (k‑1)/2, if k odd
 *            and a even @see OEIS A329321
 * Range:     u8 n <= 146, u16 n <= 8869, u32 n <= 8869, u64 n <= 8869,
 *            u128 n <= 8869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return total number of odd parts in all partitions of n into consecutive parts. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; ; ++k) {
        /* tmp = k*(k-1)/2 */
        seq_t tmp = (seq_t)k * (k - 1) / 2;
        if (tmp >= n) {   /* since a must be > 0 */
            break;
        }
        idx_t rem = n - (idx_t)tmp;   /* n - k*(k-1)/2 */
        if ((rem % k) != 0) {
            continue;   /* a not integer */
        }
        idx_t a_start = rem / k;
        if (a_start == 0) {   /* start must be at least 1 */
            continue;
        }
        /* Count odd parts in the arithmetic progression a_start,...,a_start+k-1 */
        seq_t odds;
        if ((k & 1) == 0) {           /* k even */
            odds = k / 2;
        } else {                       /* k odd */
            if (a_start & 1) {         /* a_start odd */
                odds = (k + 1) / 2;
            } else {                   /* a_start even */
                odds = (k - 1) / 2;
            }
        }
        total += odds;
    }
    return total;
}
