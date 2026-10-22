/*
 * A318968  Expansion of exp(Sum_{k>=1} ( Sum_{d|k, d odd} d^k ) * x^k/k).
 * https://oeis.org/A318968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A318968: Expansion of exp(∑_{k≥1} (∑_{d|k,d odd} d^k) * x^k / k)
   Recurrence: a(0)=1, a(n)= (1/n) * Σ_{k=1..n} S_k * a(n-k),
   where S_k = Σ_{d|k, d odd} d^k. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_pow.h"

static void grow_seq_array(seq_t **arr, size_t *cap, idx_t need)
{
    if ((size_t)need < *cap)
        return;
    size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
    while (new_cap <= (size_t)need)
        new_cap <<= 1;
    seq_t *new_arr = seq_alloc(new_cap);
    size_t i;
    /* copy old data */
    for (i = 0; i < *cap; ++i)
        new_arr[i] = (*arr)[i];
    /* zero the rest */
    for (; i < new_cap; ++i)
        new_arr[i] = 0;
    *arr = new_arr;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;           /* coefficients a(n)   */
    static size_t A_cap = 0;
    static seq_t *S = NULL;           /* auxiliary S_k values */
    static size_t S_cap = 0;
    size_t i, k;
    seq_t sum;

    /* ensure capacity for needed indices */
    grow_seq_array(&A, &A_cap, n);
    grow_seq_array(&S, &S_cap, n);

    /* base case */
    if (A[0] == 0)
        A[0] = (seq_t)1;

    /* compute S_k for k up to n on demand */
    for (k = 1; k <= (idx_t)n; ++k) {
        if (S[k] == 0) {
            seq_t sk = 0;
            /* loop over all divisors up to sqrt(k) */
            for (idx_t d = 1; d * d <= (idx_t)k; ++d) {
                if ((k % d) == 0) {
                    if ((d & 1) != 0)
                        sk += seq_pow((seq_t)d, (idx_t)k);
                    idx_t q = k / d;
                    if (q != d && ((q & 1) != 0))
                        sk += seq_pow((seq_t)q, (idx_t)k);
                }
            }
            S[k] = sk;
        }
    }

    /* compute a[n] using recurrence */
    for (i = 1; i <= (idx_t)n; ++i) {
        if (A[i] == 0) {
            seq_t acc = 0;
            for (k = 1; k <= (idx_t)i; ++k)
                acc += S[k] * A[i - k];
            A[i] = acc / (seq_t)i;     /* division is exact */
        }
    }

    return A[n];
}
