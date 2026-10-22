/*
 * A337122  a(n) is the smallest positive integer k for which the mod-k
 *          Collatz orbit has the same length as the ordinary Collatz orbit.
 * https://oeis.org/A337122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6170, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest k > 1 such that the length of the mod‑k Collatz orbit of n
   equals the ordinary Collatz orbit length of n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_next.h"

/* next value in the ordinary Collatz sequence */
static inline seq_t collatz_next(seq_t v)
{
    return (v & 1) ? v * 3 + 1 : v / 2;
}

seq_t a(idx_t n)
{
    /* ---------- ordinary Collatz orbit length ---------- */
    const seq_t start = (seq_t)n;

    /* Floyd cycle-finding to avoid large tables */
    seq_t tortoise = collatz_next(start);
    seq_t hare = collatz_next(collatz_next(start));
    while (tortoise != hare) {
        tortoise = collatz_next(tortoise);
        hare = collatz_next(collatz_next(hare));
    }

    /* find start of cycle (mu) */
    size_t mu = 0;
    tortoise = start;
    while (tortoise != hare) {
        tortoise = collatz_next(tortoise);
        hare = collatz_next(hare);
        mu++;
    }

    /* find cycle length (lambda) */
    size_t lambda = 1;
    hare = collatz_next(tortoise);
    while (tortoise != hare) {
        hare = collatz_next(hare);
        lambda++;
    }

    const size_t L = mu + lambda;   /* ordinary orbit length */

    /* ---------- search for minimal k ---------- */
    for (idx_t k = 2; ; ++k) {
        const seq_t kseq = (seq_t)k;          /* cast to seq_t for operators */
        const seq_t r0 = ((seq_t)n) % kseq;

        /* Floyd cycle detection for the mod‑k orbit */
        seq_t t = mod_next(r0, kseq);
        seq_t h = mod_next(mod_next(r0, kseq), kseq);
        while (t != h) {
            t = mod_next(t, kseq);
            h = mod_next(mod_next(h, kseq), kseq);
        }

        /* find mu2 (preperiod) */
        size_t mu2 = 0;
        t = r0;
        while (t != h) {
            t = mod_next(t, kseq);
            h = mod_next(h, kseq);
            mu2++;
        }

        /* find lambda2 (period) */
        size_t lambda2 = 1;
        h = mod_next(t, kseq);
        while (t != h) {
            h = mod_next(h, kseq);
            lambda2++;
        }

        const size_t len2 = mu2 + lambda2;   /* length of mod‑k orbit */

        if (len2 == L) {
            return kseq;
        }
    }
}
