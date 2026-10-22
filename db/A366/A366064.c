/*
 * A366064  Record values of A366091.
 * https://oeis.org/A366064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A366064: record values of the number of representations of n as
 * i^2 + 2*j^2 + 3*k^2 (i,j,k ≥ 0).
 * Computes a(n) = A366091(A366065(n)).
 * The code repeatedly doubles an upper bound B, counts all
 * representations up to B, tracks record counts, and
 * memoises all found records so that subsequent calls are fast.
 *
 * The implementation uses dynamic allocation that works both in
 * C11 (malloc/calloc) and C++17 (new[]).  For arbitrary‑precision
 * seq_t values this ensures constructors are called.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* For C++ we need to use new/delete; for C we use calloc/free. */
#ifndef __cplusplus
# define ALLOC_SEQ_T(dst, sz) \
    ((dst) = (seq_t*)calloc((size_t)(sz), sizeof(seq_t)))
# define FREE_SEQ_T(ptr) free((void*)(ptr))
# define ALLOC_SEQ_T_ZERO(dst, sz) \
    ((dst) = (seq_t*)calloc((size_t)(sz), sizeof(seq_t)))
#else
# define ALLOC_SEQ_T(dst, sz) \
    ((dst) = new seq_t[(size_t)(sz)])
# define FREE_SEQ_T(ptr) delete[] (ptr)
# define ALLOC_SEQ_T_ZERO(dst, sz) \
    ((dst) = new seq_t[(size_t)(sz)]())
#endif

/* Static cache of record counts */
static seq_t *record_cache = NULL;
static size_t record_cache_len = 0;

/* ------------------------------------------------------------------ */
/*  Compute a(n)  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is always ≥ 1 in the OEIS sequence */
    if (n == 0)
        return 0;

    /* Return cached result if already computed */
    if (record_cache_len >= (size_t)n)
        return record_cache[n - 1];

    /* Bound of values considered; start small and double as needed. */
    idx_t B = 100;

    for (;;) {
        /* Allocate zeroed count array cnt[0..B] */
        seq_t *cnt = NULL;
        ALLOC_SEQ_T_ZERO(cnt, B + 1);
        if (!cnt) return 0;   /* allocation failed */

        /* Enumerate all triples (i,j,k) and count representations. */
        for (idx_t i = 0; ; ++i) {
            uint64_t i2 = (uint64_t)i * (uint64_t)i;
            if (i2 > (uint64_t)B) break;

            for (idx_t j = 0; ; ++j) {
                uint64_t s = i2 + 2ULL * (uint64_t)j * (uint64_t)j;
                if (s > (uint64_t)B) break;

                for (idx_t k = 0; ; ++k) {
                    uint64_t v = s + 3ULL * (uint64_t)k * (uint64_t)k;
                    if (v > (uint64_t)B) break;
                    cnt[(size_t)v] += 1;
                }
            }
        }

        /* Temporary array to hold record counts found under this bound. */
        seq_t *rec = NULL;
        ALLOC_SEQ_T(rec, n);
        if (!rec) {
            FREE_SEQ_T(cnt);
            return 0;
        }

        seq_t best = 0;
        size_t seen = 0;
        for (idx_t v = 1; v <= B && seen < (size_t)n; ++v) {
            if (cnt[(size_t)v] > best) {
                best = cnt[(size_t)v];
                rec[seen++] = best;
            }
        }

        FREE_SEQ_T(cnt);   /* counts no longer needed */

        /* If we have enough records, store them in the cache and return. */
        if (seen == (size_t)n) {
            FREE_SEQ_T(record_cache);   /* discard old cache */
            record_cache      = rec;
            record_cache_len  = n;
            return record_cache[n - 1];
        }

        /* Not enough records yet.  Free rec and try with a larger B. */
        FREE_SEQ_T(rec);
        if (B > ((idx_t)-1) / 2)   /* avoid overflow */
            return 0;
        B <<= 1;   /* double the bound */
    }
}
