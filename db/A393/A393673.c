/*
 * A393673  a(n) = number of partitions of n into distinct parts that are
 *          triangular numbers, no two of which are consecutive triangular
 *          numbers.
 * https://oeis.org/A393673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct triangular
 *            numbers with no two triangular numbers that are consecutive
 * Range:     u8 n <= 576, u16 n <= 2056, u32 n <= 9048, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Allocation helpers that work in both C and C++. */
#ifdef __cplusplus
#define ALLOC_SEQ(cnt)  new seq_t[(cnt)]
#define FREE_SEQ(ptr)   delete[] (ptr)
#else
#define ALLOC_SEQ(cnt)  ((seq_t*)calloc((cnt), sizeof(seq_t)))
#define FREE_SEQ(ptr)   free((ptr))
#endif

/* Return count of partitions of n into distinct non‑consecutive
   triangular numbers. */
seq_t a(idx_t n)
{
    /* dp0[j] : ways to sum to j when the last considered triangular
       number has NOT been used; dp1[j] : ways when it has been used. */
    seq_t *dp0 = ALLOC_SEQ(n + 1);
    seq_t *dp1 = ALLOC_SEQ(n + 1);
    if (!dp0 || !dp1) return 0;                /* memory failure */

    dp0[0] = 1;                                 /* empty sum */

    for (idx_t k = 1;; ++k) {
        /* triangular number T_k = k(k+1)/2 */
        idx_t T = k * (k + 1) / 2;
        if (T > n) break;

        seq_t *ndp0 = ALLOC_SEQ(n + 1);
        seq_t *ndp1 = ALLOC_SEQ(n + 1);
        if (!ndp0 || !ndp1) {
            FREE_SEQ(dp0); FREE_SEQ(dp1);
            return 0;
        }

        for (idx_t j = 0; j <= n; ++j) {
            ndp0[j] = dp0[j] + dp1[j];          /* keep previous state */
            if (j >= T) ndp1[j] = dp0[j - T];  /* take this triangular */
            /* otherwise ndp1[j] remains zero (calloc/constructor) */
        }

        FREE_SEQ(dp0); FREE_SEQ(dp1);
        dp0 = ndp0; dp1 = ndp1;
    }

    seq_t result = dp0[n] + dp1[n];
    FREE_SEQ(dp0); FREE_SEQ(dp1);
    return result;
}
