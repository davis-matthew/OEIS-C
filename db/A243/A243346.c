/*
 * A243346  a(1) = 1, a(2n) = A005117(1+a(n)), a(2n+1) = A013929(a(n)), where
 *          A005117 are squarefree and A013929 are nonsquarefree numbers.
 * https://oeis.org/A243346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2n) = A005117(1 + a(n)) // k‑th square‑free number
 *            a(2n+1) = A013929(a(n)) // k‑th non‑square‑free number (A005117:
 *            1,2,3,5,6,7,…; A013929: 4,8,9,12,16,…) The implementation
 *            memoises computed values and builds the square‑free /
 *            nonsquare‑free tables on demand
 * Range:     u8 n <= 90, u16 n <= 5630, u32 n <= 8189, u64 n <= 8189,
 *            u128 n <= 8189, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- helper: make sure there is room for at least 'needed' elements ----- */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* --- test for square‑freeness (works for values that fit in uint64_t) ----- */
static bool is_squarefree(uint64_t x)
{
    if (x <= 1)
        return true;                     /* 1 is square‑free by convention */
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            x /= p;
            if (x % p == 0)
                return false;            /* p² divides x */
        }
    }
    return true;
}

/* --- tables of square‑free and nonsquare‑free numbers ------------------- */
static seq_t *squarefree    = NULL, *nonsquarefree = NULL;
static size_t sf_len = 0, sf_cap = 0;
static size_t nsq_len = 0, nsq_cap = 0;
static uint64_t last_checked = 0;          /* next integer to test */

/* Ensure that the i‑th (1‑based) square‑free / nonsquare‑free number is present */
static void ensure_lists(size_t k)
{
    while (sf_len < k || nsq_len < k) {
        ++last_checked;
        bool sq = is_squarefree(last_checked);
        if (sq) {
            if (sf_len + 1 > sf_cap)
                ensure_capacity(sf_len + 1, &sf_cap, &squarefree);
            squarefree[sf_len++] = (seq_t)last_checked;
        } else {
            if (nsq_len + 1 > nsq_cap)
                ensure_capacity(nsq_len + 1, &nsq_cap, &nonsquarefree);
            nonsquarefree[nsq_len++] = (seq_t)last_checked;
        }
    }
}

/* --- cache for the recursive computation --------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0, cache_len = 0;

/* -------------------------------------------------------------------------- */
/*  Required entry point:  seq_t a(idx_t n)   (external linkage)              */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Grow cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t needed = n + 1;
        if (needed > cache_cap)
            ensure_capacity(needed, &cache_cap, &cache);
        while (cache_len < needed)
            cache[cache_len++] = (seq_t)0;   /* 0 ⇒ not yet computed */
    }
    if (cache[n] != 0)
        return cache[n];                     /* already computed */

    idx_t m = n >> 1;                         /* floor(n/2) */
    seq_t am = a(m);                          /* recursive call – memoised */

    seq_t result;
    if (n & 1U) {                            /* odd: a(2m+1) = nonsquarefree[am] */
        size_t k = (size_t)am;                /* 1‑based index */
        ensure_lists(k);
        result = nonsquarefree[k - 1];
    } else {                                  /* even: a(2m)   = squarefree[am+1] */
        size_t k = (size_t)(am + 1U);          /* 1‑based index */
        ensure_lists(k);
        result = squarefree[k - 1];
    }

    cache[n] = result;
    return result;
}
