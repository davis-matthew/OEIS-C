/*
 * A309628  Number of odd parts in the partitions of n into 8 parts.
 * https://oeis.org/A309628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all partitions of n into exactly 8 parts of the
 *            number of odd parts in the partition. This implementation uses a
 *            recurrence on partitions with a fixed number of parts: p_k(n) =
 *            p_k(n-k) + p_{k-1}(n-1) f_k(n) = k·p_k(n-k) – f_k(n-k) +
 *            f_{k-1}(n-1) + p_{k-1}(n-1) where p_k(n) is the...
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Allocation helpers that work both for plain integer types (C) and for
   arbitrary‑precision integer classes (C++). */
#ifdef __cplusplus
#define MALLOC(sz)   new (std::nothrow) seq_t[(sz)]
#define FREE(ptr)    delete [] (ptr)
#else
#define MALLOC(sz)   ((seq_t *)malloc((sz)*sizeof(seq_t)))
#define FREE(ptr)    free((ptr))
#endif

seq_t a(idx_t n)
{
    /* Static arrays holding the tables of counts and odd‑part totals.  */
    static seq_t *p_arr = NULL;   /* number of partitions */
    static seq_t *f_arr = NULL;   /* total odd parts in those partitions */
    static idx_t  max_n = 0;      /* largest value previously computed */

    size_t N = (size_t)n;

    /* Recompute tables if we have not yet handled this n. */
    if (p_arr == NULL || N > max_n)
    {
        size_t stride     = N + 1;              /* columns: sum from 0..N */
        size_t table_size = 9 * stride;         /* 9 rows: k = 0..8 */

        if (p_arr) FREE(p_arr);
        if (f_arr) FREE(f_arr);

        p_arr = MALLOC(table_size);
        f_arr = MALLOC(table_size);
        if (!p_arr || !f_arr)                 /* allocation failure */
            return (seq_t)0;                   /* safe fallback */

        /* Initialise all entries to zero. */
        for (size_t i = 0; i < table_size; ++i) {
            p_arr[i] = (seq_t)0;
            f_arr[i] = (seq_t)0;
        }

        /* Base case: partitions of 0 into 0 parts. */
        p_arr[0] = (seq_t)1;   /* p_0(0) = 1,  f_0(0) = 0 already set */

        /* DP over n and k. */
        for (size_t s = 1; s <= N; ++s) {
            for (int k = 1; k <= 8; ++k) {
                if (s < (idx_t)k) {              /* not enough sum for k parts */
                    continue;
                }

                /* Indices for the two preceding terms. */
                size_t idx_k         = k * stride;
                size_t idx_k_minus1  = (k - 1) * stride;

                seq_t pk_n_k  = p_arr[idx_k     + (s - k)];   /* p_k(s-k)   */
                seq_t fk_n_k  = f_arr[idx_k     + (s - k)];   /* f_k(s-k)   */
                seq_t pkm1_n1 = p_arr[idx_k_minus1 + (s - 1)];/* p_{k-1}(s-1)*/
                seq_t fkm1_n1 = f_arr[idx_k_minus1 + (s - 1)];/* f_{k-1}(s-1)*/

                /* p_k(s) */
                p_arr[idx_k + s] = pk_n_k + pkm1_n1;

                /* f_k(s) = k·p_k(s-k) – f_k(s-k) + f_{k-1}(s-1) + p_{k-1}(s-1) */
                f_arr[idx_k + s] =
                    ((seq_t)k * pk_n_k - fk_n_k) + (fkm1_n1 + pkm1_n1);
            }
        }
        max_n = N;
    }

    /* Length of each row in the tables. */
    size_t stride = max_n + 1;
    /* Return the desired value: total odd parts in partitions of n into 8 parts. */
    return f_arr[8 * stride + N];
}
