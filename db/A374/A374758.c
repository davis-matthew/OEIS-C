/*
 * A374758  Sum of leaders of strictly decreasing runs in the n-th composition
 *          in standard order.
 * https://oeis.org/A374758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of leaders of strictly decreasing runs in the
 * n‑th composition in standard order.
 *
 * A composition of k is listed in standard order (lexicographic with
 * ascending first parts).  The n‑th composition (n≥1) belongs to an
 * integer k where 2^{k‑1} ≤ n ≤ 2^k – 1.  The offset within the
 * compositions of k is n – 2^{k‑1}.  The composition is then
 * reconstructed by a greedy descent that selects each part by comparing
 * the offset against the number of compositions of the remainder.
 *
 * The sum of leaders is obtained by taking the first part of the
 * composition and adding every part that is not smaller than the part
 * immediately preceding it (i.e., it starts a new strictly decreasing
 * run).  The first part is always a leader.
 *
 * a(0) is defined as 0 as a placeholder.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset for the starting term */
    if (n == 0)
        return (seq_t)0;

    /* Determine k = floor_log2(n) + 1 (number of bits of n) */
    idx_t tmp = n;
    idx_t k = 0;
    while (tmp) {
        tmp >>= 1;
        ++k;
    }

    /* 2^{k-1} is the starting rank of compositions of size k */
    idx_t pow = ((idx_t)1) << (k - 1);

    /* Zero‑based offset inside compositions of k */
    idx_t offset = n - pow;

    /* Reconstruct the composition */
    int parts[64];          /* maximum length for k≤64 */
    int pos = 0;
    int rem = (int)k;

    while (rem > 0) {
        for (int first = rem; first >= 1; --first) {
            int new_rem = rem - first;
            idx_t count;
            if (new_rem == 0)
                count = 1;
            else
                count = ((idx_t)1) << (new_rem - 1);  /* compositions of new_rem */
            if (offset < count) {
                parts[pos++] = first;
                rem = new_rem;
                break;
            } else {
                offset -= count;
            }
        }
    }

    /* Sum leaders of strictly decreasing runs */
    seq_t sum = 0;
    if (pos > 0) {
        sum += (seq_t)parts[0];
        for (int i = 1; i < pos; ++i) {
            if (parts[i] >= parts[i - 1])   /* start of a new run */
                sum += (seq_t)parts[i];
        }
    }
    return sum;
}
