/*
 * A267534  Indices of Lucas numbers that are not of the form x^2 + y^2 + z^2
 *          where x, y and z are integers.
 * https://oeis.org/A267534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th index k for which Lucas(k) is of the form 4^a *
 *            (8b+7). The function checks this using Legendre's theorem (three
 *            squares theorem): a positive integer is not a sum of three
 *            squares iff it is of that form. The Lucas numbers are generated
 *            iteratively modulo 2^64; this is sufficient b...
 * Range:     u8 n <= 73, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   must_fail_three_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/must_fail_three_square.h"

/* The sequence function.  It memoises computed terms so calls may
   arrive in any order.  All computations use only integer arithmetic. */
seq_t a(idx_t n)
{
    /* Static state initially uninitialised, set on first call. */
    static bool init = false;
    static seq_t *cache = NULL;
    static size_t clen = 0, ccap = 0;

    /* Lucas number state.  lucas_prev  = L(k-1), lucas_cur = L(k). */
    static unsigned long long lucas_prev_prev = 2ULL; /* L0 */
    static unsigned long long lucas_prev        = 1ULL; /* L1 */
    static idx_t lucas_last = 1;                       /* last k processed */

    if (!init) {
        init = true;
        /* initial capacity */
        ccap = 64;
        cache = (seq_t*)malloc(ccap * sizeof(seq_t));

        /* handle k = 1 immediately – value is 1 which is representable */
        if (must_fail_three_square(lucas_prev)) {
            if (clen >= ccap) { ccap <<= 1; cache = (seq_t*)realloc(cache, ccap * sizeof(seq_t)); }
            cache[clen++] = (seq_t)1;
        }
        /* lucas_last is already 1, lucas_prev holds L1. */
    }

    /* grow cache until we have at least n terms */
    while (clen < n) {
        /* compute next Lucas number L(k) for k = lucas_last + 1 */
        idx_t k = lucas_last + 1;
        unsigned long long lk = lucas_prev + lucas_prev_prev;

        /* shift state for next iteration */
        lucas_prev_prev = lucas_prev;
        lucas_prev = lk;
        lucas_last = k;

        /* if L(k) cannot be expressed as sum of three squares, store k */
        if (must_fail_three_square(lk)) {
            if (clen >= ccap) {
                ccap <<= 1;
                cache = (seq_t*)realloc(cache, ccap * sizeof(seq_t));
            }
            cache[clen++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
