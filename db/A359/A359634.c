/*
 * A359634  a(0)=1 and thereafter a(n) is the length of the longest contiguous
 *          group of terms in the sequence thus far that add up to n; if no
 *          such group exists, set a(n)=0.
 * https://oeis.org/A359634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) is the length (counting only * non‑zero
 *            terms) of the longest contiguous subarray of the * sequence
 *            a[0..n‑1] that sums to n
 * Range:     u8 n <= 1160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;           /* stored terms of the sequence   */
static idx_t  seq_len = 0;          /* number of terms already computed */
static size_t seq_cap = 0;          /* current allocated capacity */

/* Ensure the array has room for at least one element */
static void seq_init(void)
{
    if (seq == NULL) {
        seq_cap = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
        seq[0] = (seq_t)1;          /* a(0) = 1 */
        seq_len = 1;
    }
}

seq_t a(idx_t n)
{
    seq_init();                      /* make sure array exists          */

    if (n == 0) return seq[0];       /* base case is already stored     */

    /* Extend the sequence up to index n */
    for (idx_t k = seq_len; k <= n; ++k) {
        seq_t target = (seq_t)k;     /* sum that we are looking for    */
        seq_t curSum  = 0;
        idx_t  nonZero = 0;          /* number of non‑zero terms in current window */
        idx_t  best    = 0;
        idx_t  left    = 0;

        /* Two‑pointer scan over a[0..k-1] looking for subarrays
           summing to target.  The array uses only non‑negative
           integers, so the classic sliding‑window algorithm applies. */
        for (idx_t right = 0; right < k; ++right) {
            seq_t val = seq[right];
            curSum += val;
            if (val != (seq_t)0) nonZero++;

            while (curSum > target && left <= right) {
                seq_t vl = seq[left];
                curSum -= vl;
                if (vl != (seq_t)0) nonZero--;
                ++left;
            }

            if (curSum == target) {
                if (nonZero > best) best = nonZero;
            }
        }

        /* Append the newly found term */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!seq) exit(EXIT_FAILURE);
        }
        seq[seq_len++] = (seq_t)best;
    }

    return seq[n];
}
