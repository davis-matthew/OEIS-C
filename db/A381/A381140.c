/*
 * A381140  Expansion of e.g.f. exp( -LambertW(-x * cosh(x)) ).
 * https://oeis.org/A381140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies A(x) = exp( x * cosh(x) * A(x) ). Let b(n) =
 *            Σ_{odd k≤n} C(n,k) · k · a(n‑k) (b(0)=0). Then a(0)=1 and for
 *            n≥1 a(n) = Σ_{k=1..n} C(n‑1,k‑1) · b(k) · a(n‑k). The code below
 *            computes these values iteratively, using only integer
 *            arithmetic; it works for any seq_t type supplied by the...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays for a[0..n] and b[0..n] */
    seq_t *a_arr = alloc_seq((size_t)n + 1);
    seq_t *b_arr = alloc_seq((size_t)n + 1);

    a_arr[0] = (seq_t)1;
    b_arr[0] = (seq_t)0;

    for (idx_t N = 1; N <= n; ++N)
    {
        /* ---- compute b(N) ---- */
        seq_t sum_b = (seq_t)0;
        seq_t bin = (seq_t)1;            /* binom(N,0) */
        for (idx_t k = 1; k <= N; ++k)
        {
            bin = bin * (seq_t)(N - k + 1) / (seq_t)k;   /* binom(N,k) */
            if ((k & 1U) == 1)            /* k is odd, f_k = k */
            {
                seq_t term = bin * a_arr[N - k];
                sum_b += term * (seq_t)k;
            }
        }
        b_arr[N] = sum_b;

        /* ---- compute a(N) ---- */
        seq_t sum_a = (seq_t)0;
        seq_t binprev = (seq_t)1;         /* binom(N-1,0) */
        for (idx_t k = 1; k <= N; ++k)
        {
            seq_t term = binprev * b_arr[k];
            term = term * a_arr[N - k];
            sum_a += term;
            if (k < N)                    /* update for next k+1 */
                binprev = binprev * (seq_t)(N - k) / (seq_t)k;
        }
        a_arr[N] = sum_a;
    }

    seq_t result = a_arr[n];

    free(a_arr);
    free(b_arr);
    return result;
}
