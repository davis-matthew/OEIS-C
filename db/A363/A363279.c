/*
 * A363279  a(0)=1; a(1)=2. For n>1, a(n) is the number of contiguous groups
 *          in the sequence thus far whose sum is n.
 * https://oeis.org/A363279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of contiguous subarrays of the sequence up to index n-1
 *            whose sum equals n. a(0)=1, a(1)=2, a(n)=# { i<=j<n :
 *            sum_{k=i}^{j} a(k) = n } (n>1)
 * Range:     u8 n <= 294, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types supplied by the build. */
extern seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *seq = NULL;
    static idx_t len = 0;   /* number of terms currently stored (next free index) */
    static idx_t cap = 0;   /* allocated capacity */

    /* Ensure capacity for up to index n. */
    if (cap == 0) {
        cap = (n > 1) ? n + 1 : 2;
        seq = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;                 /* allocation failed */
    } else if (n + 1 > cap) {
        idx_t newcap = cap << 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(seq, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        seq = tmp;
        cap = newcap;
    }

    /* Compute terms up to n lazily. */
    while (len <= n) {
        if (len == 0) {
            seq[0] = (seq_t)1;
        } else if (len == 1) {
            seq[1] = (seq_t)2;
        } else {
            /* Count subarrays of seq[0..len-1] that sum to target = len. */
            idx_t target_idx = len;
            seq_t target = (seq_t)target_idx;
            idx_t left = 0, right = 0;
            seq_t cur = (seq_t)0;
            seq_t count = (seq_t)0;

            while (right < len) {
                cur += seq[right];
                while (cur > target && left <= right) {
                    cur -= seq[left];
                    ++left;
                }
                if (cur == target) {
                    ++count;
                }
                ++right;
            }
            seq[len] = count;
        }
        ++len;
    }

    return seq[n];
}
