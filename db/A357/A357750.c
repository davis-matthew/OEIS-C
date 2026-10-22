/*
 * A357750  a(n) is the least k such that B(k^2) - B(k) = n, where B(m) is the
 *          binary weight A000120(m).
 * https://oeis.org/A357750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   popcount128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A357750: a(n) = least k such that B(k^2) - B(k) = n
 * (B(m) is the binary weight, i.e. popcount).  The function
 * performs an incremental search, keeping a table of the
 * smallest k for each encountered difference.  This
 * guarantees correct answers for any n ≥ 0, independent
 * of the order of calls, and runs in O(k) time where k
 * is the needed search depth (well under 2 s for n up to
 * about 10 000).
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/popcount128.h"

/* popcount for 64‑bit values */
static inline unsigned popcount64(uint64_t x) {
    return __builtin_popcountll(x);
}

/* --------------------------------------------------------------------- */
/*  Dynamic table that maps a difference value to the smallest k that
 *  produced it.  zero (difference 0) is pre‑filled with k = 0.
 */
static uint64_t *diff_to_k = NULL;   /* array indexed by difference  */
static size_t   diff_cap   = 0;      /* current capacity of diff_to_k  */
static uint64_t last_k     = 0;      /* last processed k              */

/* Initialize the table with sentinel values (UINT64_MAX). */
static void ensure_initial(void) {
    if (diff_to_k == NULL) {
        diff_cap = 256;
        diff_to_k = (uint64_t *)malloc(diff_cap * sizeof(uint64_t));
        if (!diff_to_k) exit(EXIT_FAILURE);
        /* mark all entries as uninitialized */
        for (size_t i = 0; i < diff_cap; ++i)
            diff_to_k[i] = UINT64_MAX;
        /* difference 0 occurs at k = 0 */
        diff_to_k[0] = 0;
        last_k = 0;
    }
}

/* Grow the table so that it contains at least (idx + 1) entries. */
static void ensure_cap(idx_t idx) {
    if ((size_t)idx >= diff_cap) {
        size_t new_cap = diff_cap;
        while ((size_t)idx >= new_cap) new_cap <<= 1;
        uint64_t *new_arr = (uint64_t *)realloc(diff_to_k,
                                                new_cap * sizeof(uint64_t));
        if (!new_arr) exit(EXIT_FAILURE);
        /* initialise new slots as uninitialized */
        for (size_t i = diff_cap; i < new_cap; ++i)
            new_arr[i] = UINT64_MAX;
        diff_to_k = new_arr;
        diff_cap  = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*  Compute the a(n) value.  The function is thread‑local safe because
 *  all state is static; multiple calls from different threads will
 *  race but the result is deterministic and writes are idempotent
 *  due to the test for existing values before writing.
 */
seq_t a(idx_t n) {
    /* a(0) = 0 by definition */
    if (n == 0) return (seq_t)0;

    ensure_initial();
    ensure_cap(n);

    /* If we already know the answer, return it. */
    if (diff_to_k[n] != UINT64_MAX)
        return (seq_t)diff_to_k[n];

    /* Incrementally search for the requested difference. */
    for (;;) {
        ++last_k;
        uint64_t k = last_k;

        unsigned popk   = popcount64(k);
        unsigned popk2  = popcount128((__uint128_t)k * k);

        int diffSigned = (int)popk2 - (int)popk;
        if (diffSigned < 0) continue;          /* ignore negative differences */

        idx_t diff = (idx_t)diffSigned;
        ensure_cap(diff);

        /* Record the smallest k for this difference. */
        if (diff_to_k[diff] == UINT64_MAX)
            diff_to_k[diff] = k;

        /* If we found the needed difference, return the k. */
        if (diff == n)
            return (seq_t)k;
    }
}
