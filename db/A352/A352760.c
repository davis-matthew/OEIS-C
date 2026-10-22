/*
 * A352760  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, among the ternary digits of n
 *          and a(n) (counted with multiplicity) there are as many 1's as 2's.
 * https://oeis.org/A352760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the lexicographically earliest permutation of the
 *            non‑negative integers such that f(n)+f(a(n)) = 0, where f(x) =
 *            (# of digits equal to 1 in the base‑3 expansion of x) – (# of
 *            digits equal to 2 in that expansion). Consequently a(n)=n when
 *            f(n)=0; for f(n)≠0 we pair each i with the smallest...
 * Range:     u8 n <= 243, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   seq_realloc, ternary_balance
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"   /* reallocation helper */
#include "../utils/ternary_balance.h"

/* Maximum absolute value of f(x) for 64‑bit numbers: 40.  Add safety margin. */
#define F_OFFSET 50
#define F_ARRAY_SIZE (2*F_OFFSET+1)

/* Static state shared across calls */
static idx_t      cache_size      = 0;    /* allocated number of slots         */
static seq_t    *a_cache          = NULL; /* permutation values                */
static bool     *used_cache       = NULL; /* whether slot is already paired    */
static int      *f_cache          = NULL; /* cached f(x) values                */
static idx_t    processed_limit  = 0;      /* indices processed so far          */
static idx_t     next_candidate[F_ARRAY_SIZE];   /* first unused index for each f  */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Ensure the internal arrays can hold indices up to 'need'          */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_size)
        return;                           /* nothing to do */

    /* Reallocate permutation array */
    seq_t *tmp_cache = seq_realloc(a_cache, cache_size, need);
    if (!tmp_cache) exit(1);
    a_cache = tmp_cache;

    /* Reallocate used flag array */
    bool *tmp_used = (bool *)realloc(used_cache, need * sizeof(bool));
    if (!tmp_used) exit(1);
    used_cache = tmp_used;

    /* Reallocate f_cache array */
    int *tmp_f = (int *)realloc(f_cache, need * sizeof(int));
    if (!tmp_f) exit(1);
    f_cache = tmp_f;

    /* Initialise new slots and recompute ternary balances */
    for (idx_t i = cache_size; i < need; ++i) {
        used_cache[i] = false;
        f_cache[i]    = ternary_balance(i);
    }

    cache_size = need;

    /* Initialise next_candidate array once (for all f values) */
    static bool init_flag = false;
    if (!init_flag) {
        for (int k = 0; k < F_ARRAY_SIZE; ++k)
            next_candidate[k] = 0;
        init_flag = true;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function: returns a(n)                                      */
seq_t a(idx_t n)
{
    /* Find the smallest power of 3 that is greater than n     */
    idx_t p = 1;
    while (p <= n) {
        if (p > UINT64_MAX / 3) {   /* avoid overflow */
            p = UINT64_MAX;
            break;
        }
        p *= 3;
    }
    idx_t maxIndex = p - 1;         /* all pairs of indices ≤ maxIndex stay below p */

    /* Grow helper arrays if necessary */
    ensure_capacity(maxIndex + 1);

    /* Process indices that are not yet paired                 */
    for (idx_t i = processed_limit; i <= maxIndex; ++i) {
        if (used_cache[i])
            continue;                /* already paired */

        int fi = f_cache[i];
        if (fi == 0) {               /* self‑pair */
            a_cache[i]   = (seq_t)i;
            used_cache[i] = true;
            /* advance next_candidate for f=0                          */
            int idx0 = next_candidate[F_OFFSET];
            while (idx0 <= maxIndex && used_cache[idx0])
                ++idx0;
            next_candidate[F_OFFSET] = idx0;
        } else {
            int target = -fi;
            int idx    = next_candidate[target + F_OFFSET];
            /* Find the smallest unused index with f = target */
            while (idx <= maxIndex &&
                   (used_cache[idx] || f_cache[idx] != target))
                ++idx;

            if (idx > maxIndex) {
                /* This should never happen for valid n, but guard anyway */
                exit(1);
            }

            /* Pair i with idx */
            a_cache[i]   = (seq_t)idx;
            a_cache[idx] = (seq_t)i;
            used_cache[i] = true;
            used_cache[idx] = true;

            /* Update next_candidate for 'target' f value            */
            idx += 1;
            while (idx <= maxIndex && used_cache[idx])
                ++idx;
            next_candidate[target + F_OFFSET] = idx;
        }
    }

    processed_limit = maxIndex + 1;
    return a_cache[n];
}
