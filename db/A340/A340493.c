/*
 * A340493  Sequence whose partial sums give A340492.
 * https://oeis.org/A340493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n)*A000070(n-1) - A000041(n-1)*A000070(n-2) where
 *            A000041(0)=1, A000041(n) is the partition number p(n) and
 *            A000070(n)=∑_{k=0..n} p(k). a(1)=1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_arr = NULL;      /* p[n] = partition number p(n), p[0] = 1 */
static seq_t *q_arr = NULL;      /* q[n] = A000070(n), q[0] = 1 */
static idx_t  max_n   = 0;        /* largest n for which arrays are computed */

/* Compute p[0..N] and q[0..N], extending from current max_n if necessary. */
static void compute_up_to(idx_t N)
{
    if (N <= max_n) return;             /* already computed */

    /* Allocate or resize the arrays to hold indices up to N. */
    seq_t *new_p = (seq_t *)realloc(p_arr, (N + 1) * sizeof(seq_t));
    seq_t *new_q = (seq_t *)realloc(q_arr, (N + 1) * sizeof(seq_t));
    if (!new_p || !new_q) {
        abort();                         /* memory allocation failed */
    }
    p_arr = new_p;
    q_arr = new_q;

    /* Initialise base case if this is the very first call. */
    if (max_n == 0) {
        p_arr[0] = (seq_t)1;            /* p(0) = 1 */
        q_arr[0] = (seq_t)1;            /* q(0) = 1 */
    }

    /* Compute partition numbers p[i] for i = max_n+1 .. N using Euler's
       pentagonal recurrence: */
    for (idx_t i = max_n + 1; i <= N; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1)   pos += p_arr[i - pent1];
            else         neg += p_arr[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) {
                if (k & 1)   pos += p_arr[i - pent2];
                else         neg += p_arr[i - pent2];
            }
        }
        p_arr[i] = pos - neg;                  /* pos ≥ neg for all i ≥ 0 */
    }

    /* Compute cumulative sums q[i] = q[i-1] + p[i] for i = max_n+1 .. N. */
    for (idx_t i = max_n + 1; i <= N; ++i) {
        q_arr[i] = q_arr[i - 1] + p_arr[i];
    }

    max_n = N;                                   /* update highest index */
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;                           /* a(1) = 1 */
    }
    compute_up_to(n);                               /* ensure p and q ready */

    /* a(n) = p[n] * q[n-1] - p[n-1] * q[n-2]   for n ≥ 2 */
    seq_t term1 = p_arr[n] * q_arr[n - 1];
    seq_t term2 = p_arr[n - 1] * q_arr[n - 2];
    return term1 - term2;
}
