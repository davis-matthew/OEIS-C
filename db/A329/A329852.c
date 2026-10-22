/*
 * A329852  Numbers having twice as many 1's in their binary expansion as
 *          terms in their Zeckendorf expansion.
 * https://oeis.org/A329852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A329852: numbers having twice as many 1's in their binary
    expansion as terms in their Zeckendorf expansion.
    Computation: brute‑force scan of integers; for each integer
    compute popcount and Zeckendorf term count via a greedy algorithm.
    The nth term is returned; results are cached so that calls with
    arbitrary n values are fast.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  Storage of Fibonacci numbers used for Zeckendorf  ---------- */
static seq_t *fib = NULL;          /* array of Fibonacci numbers   */
static size_t fib_len = 0;         /* number of elements in fib    */
static size_t fib_cap = 0;         /* allocated capacity           */

/* Ensure that fib contains all Fibonacci numbers ≤ x.  */
static void ensure_fib(seq_t x) {
    if (fib == NULL) {              /* first time: create base fibs  */
        fib_cap = 2;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib) { exit(1); }
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)2;
        fib_len = 2;
    }
    while (fib[fib_len-1] <= x) {
        if (fib_len == fib_cap) {   /* grow array if needed        */
            fib_cap = fib_cap ? fib_cap*2 : 2;
            fib = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
            if (!fib) { exit(1); }
        }
        fib[fib_len] = fib[fib_len-1] + fib[fib_len-2];
        fib_len++;
    }
}

/* ----------  Helper: pop‑count for a seq_t value  ---------- */
static unsigned popcount_seq_t(seq_t v) {
    unsigned c = 0;
    while (v) {
        c += (unsigned)(v & 1);
        v >>= 1;
    }
    return c;
}

/* ----------  Helper: Zeckendorf term count  ---------- */
static unsigned zeckendorf_count(seq_t v) {
    if (v == 0) return 0;
    unsigned cnt = 0;
    ensure_fib(v);
    size_t i = fib_len - 1;
    while (v > 0) {
        while (fib[i] > v) --i;
        v -= fib[i];
        ++cnt;
    }
    return cnt;
}

/* ----------  Cache of already found terms  ---------- */
static seq_t *cache = NULL;        /* cached sequence values     */
static size_t cache_len = 0;       /* number of cached values    */
static size_t cache_cap = 0;       /* allocated capacity         */

/* ----------  Main function required by the problem  ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but avoid UB         */

    /* Grow cache until it contains the nth term */
    while (cache_len < n) {
        seq_t candidate;
        if (cache_len == 0)
            candidate = (seq_t)1;
        else
            candidate = cache[cache_len-1] + (seq_t)1;

        /* Scan candidates until one satisfies the condition */
        for (;;) {
            unsigned pc = popcount_seq_t(candidate);
            unsigned zc = zeckendorf_count(candidate);
            if (pc == 2 * zc) {
                /* Store found term */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap*2 : 4;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) { exit(1); }
                }
                cache[cache_len++] = candidate;
                break;
            }
            candidate += (seq_t)1;
        }
    }

    return cache[n-1];
}
