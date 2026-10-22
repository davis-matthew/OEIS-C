/*
 * A397717  Convolution of hyperfactorials and superfactorials.
 * https://oeis.org/A397717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} A002109(k)*A000178(n-k) where
 *            A002109(k)=∏_{i=1}^k i^i (k=0 gives 1) A000178(k)=∏_{i=1}^k i!
 *            (k=0 gives 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   free_seq_array, pow_seq, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_seq.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    seq_t *tmp = seq_alloc_new_array(need);
    if (!tmp)
        return;                                /* allocation failure → caller must fail */
    for (size_t i = 0; i < *cap; ++i)
        tmp[i] = (*arr)[i];
    free_seq_array(*arr);
    *arr = tmp;
    *cap = need;
}

seq_t a(idx_t n)
{
    /* static caches for superfactorials, hyperfactorials, factorials and the sequence values */
    static seq_t *S   = NULL, *H   = NULL, *F   = NULL, *A   = NULL;
    static size_t capS = 0,      capH = 0,      capF = 0,      capA = 0;
    static idx_t  max_n = 0;
    static bool   initialized = false;

    /* initialisation for n = 0 */
    if (!initialized) {
        S   = seq_alloc_new_array(1);
        H   = seq_alloc_new_array(1);
        F   = seq_alloc_new_array(1);
        A   = seq_alloc_new_array(1);
        if (!S || !H || !F || !A)
            return (seq_t)0;    /* allocation failure – return zero */

        S[0]   = (seq_t)1;
        H[0]   = (seq_t)1;
        F[0]   = (seq_t)1;
        A[0]   = (seq_t)1;      /* a(0) = 1 */

        capS = capH = capF = capA = 1;
        max_n = 0;
        initialized = true;
    }

    /* extend caches if a larger n is requested */
    if (n > max_n) {
        size_t new_cap = (size_t)(n + 1);
        grow_seq(&S,   &capS, new_cap);
        grow_seq(&H,   &capH, new_cap);
        grow_seq(&F,   &capF, new_cap);
        grow_seq(&A,   &capA, new_cap);

        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* factorial */
            F[i] = F[i-1] * (seq_t)i;

            /* superfactorial: S[i] = S[i-1] * i! */
            S[i] = S[i-1] * F[i];

            /* hyperfactorial: H[i] = H[i-1] * i^i */
            seq_t pow_val = pow_seq((seq_t)i, (int)i);
            H[i] = H[i-1] * pow_val;

            /* convolution to get a(i) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= i; ++k)
                sum += H[k] * S[i-k];
            A[i] = sum;
        }

        max_n = n;
    }

    return A[n];
}
