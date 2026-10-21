/*
 * A292778  INVERT transform of double factorials.
 * https://oeis.org/A292778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} k!! * a(n-k), with a(0)=1. The double
 *            factorials satisfy: 0!! = 1, 1!! = 1, k!! = k * (k-2)!! for k ≥
 *            2. This implementation uses static arrays up to a maximum index
 *            of 20000, which covers the required range (≈10 000) while
 *            remaining fast. For smaller builds the same code work...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 33, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */

#ifndef MAX_A_INDEX
#define MAX_A_INDEX 20000    /* maximum n handled; 20 000 > 10 000 */
#endif

static seq_t df_cache[MAX_A_INDEX + 1]; /* double factorials */
static seq_t seq_cache[MAX_A_INDEX + 1];/* sequence values */

static idx_t df_cap = 0;   /* next df index that needs computing */
static idx_t seq_cap = 0;  /* next seq index that needs computing */

seq_t a(idx_t n)
{
    /* If requested index is larger than the static buffer, increase it
       by reallocating with a larger capacity.  In C++ we use new/delete,
       otherwise malloc/realloc.  This section is only executed when
       the array is too small for the current request. */
#if defined(__cplusplus)
    if ((size_t)n > MAX_A_INDEX) {
        static seq_t *df_dyn   = nullptr;
        static seq_t *seq_dyn  = nullptr;
        static size_t dyn_cap  = 0;

        if (n >= dyn_cap) {
            size_t new_cap = n + 1;
            seq_t *new_df  = new seq_t[new_cap];
            seq_t *new_seq = new seq_t[new_cap];
            if (df_dyn) delete [] df_dyn;
            if (seq_dyn) delete [] seq_dyn;
            df_dyn = new_df;
            seq_dyn = new_seq;
            df_cap = 0;
            seq_cap = 0;
            dyn_cap = new_cap;
        }
        /* Compute using dynamic arrays */
        while (df_cap <= n) {
            if (df_cap <= 1) {
                df_dyn[df_cap] = (seq_t)1;
            } else {
                df_dyn[df_cap] = (seq_t)df_cap * df_dyn[df_cap - 2];
            }
            ++df_cap;
        }
        while (seq_cap <= n) {
            if (seq_cap == 0) {
                seq_dyn[0] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t k = 1; k <= seq_cap; ++k)
                    sum += df_dyn[k] * seq_dyn[seq_cap - k];
                seq_dyn[seq_cap] = sum;
            }
            ++seq_cap;
        }
        return seq_dyn[n];
    }
#else
    if ((size_t)n > MAX_A_INDEX) {
        /* Fallback to dynamic memory allocation in C.  This code path
           is not expected to be hit under normal test conditions. */
        static seq_t *df_dyn   = NULL;
        static seq_t *seq_dyn  = NULL;
        static size_t dyn_cap  = 0;

        if ((size_t)n >= dyn_cap) {
            size_t new_cap = n + 1;
            seq_t *new_df  = (seq_t *)realloc(df_dyn, new_cap * sizeof(seq_t));
            seq_t *new_seq = (seq_t *)realloc(seq_dyn, new_cap * sizeof(seq_t));
            df_dyn = new_df;
            seq_dyn = new_seq;
            dyn_cap = new_cap;
            df_cap = 0;
            seq_cap = 0;
        }

        while (df_cap <= n) {
            if (df_cap <= 1) {
                df_dyn[df_cap] = (seq_t)1;
            } else {
                df_dyn[df_cap] = (seq_t)df_cap * df_dyn[df_cap - 2];
            }
            ++df_cap;
        }
        while (seq_cap <= n) {
            if (seq_cap == 0) {
                seq_dyn[0] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t k = 1; k <= seq_cap; ++k)
                    sum += df_dyn[k] * seq_dyn[seq_cap - k];
                seq_dyn[seq_cap] = sum;
            }
            ++seq_cap;
        }
        return seq_dyn[n];
    }
#endif

    /* --- static array path (used for n ≤ MAX_A_INDEX) --- */
    /* Ensure double factorial table is up to date. */
    while (df_cap <= n) {
        if (df_cap <= 1) {
            df_cache[df_cap] = (seq_t)1;
        } else {
            df_cache[df_cap] = (seq_t)df_cap * df_cache[df_cap - 2];
        }
        ++df_cap;
    }

    /* Compute sequence values up to n. */
    while (seq_cap <= n) {
        if (seq_cap == 0) {
            seq_cache[0] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= seq_cap; ++k)
                sum += df_cache[k] * seq_cache[seq_cap - k];
            seq_cache[seq_cap] = sum;
        }
        ++seq_cap;
    }

    return seq_cache[n];
}
