/*
 * A304991  a(n) = A000041(n) * A000009(n).
 * https://oeis.org/A304991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) * A000009(n) Where A000041 is the number of
 *            unrestricted partitions, and A000009 is the number of partitions
 *            into distinct parts
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Global cache for the two auxiliary sequences. */
static seq_t *p = NULL;          /* A000041 */
static seq_t *q = NULL;          /* A000009 */
static idx_t  current_size = 0;  /* largest n that has been computed */

/* Allocate new arrays and compute both sequences up to n (inclusive). */
static void compute_to(idx_t n)
{
    seq_t *p_new = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *q_new = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!p_new || !q_new) {
        if (p_new) free(p_new);
        if (q_new) free(q_new);
        abort();
    }

    /* initialise with p(0)=1, q(0)=1 and zeros elsewhere */
    p_new[0] = 1;
    q_new[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        p_new[i] = 0;
        q_new[i] = 0;
    }

    /* Ordinary partitions: pentagonal-number recurrence
       p(0)=1; for n>0: p(n)=sum_{k!=0} (-1)^{k+1} p(n- |k(3k-1)/2|)   */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            seq_t term = p_new[m - g1];
            if (k & 1) pos += term; else neg += term;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) {
                term = p_new[m - g2];
                if (k & 1) pos += term; else neg += term;
            }
        }
        p_new[m] = pos - neg;   /* guaranteed non‑negative */
    }

    /* Partitions into distinct parts: simple DP (O(n²)) */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            q_new[sum] += q_new[sum - part];
        }
    }

    /* Replace old tables with the new ones */
    free(p);
    free(q);
    p = p_new;
    q = q_new;
    current_size = n;
}

/* Return a(n) = p(n)*q(n), computing needed values on demand. */
seq_t a(idx_t n)
{
    /* Ensure the table exists for n == 0 on first call. */
    if (p == NULL && n == 0) {
        compute_to(0);
    }
    if (n > current_size) {
        compute_to(n);
    }
    return p[n] * q[n];
}
