/*
 * A265775  Denominators of upper primes-only best approximates (POBAs) to
 *          sqrt(2); see Comments.
 * https://oeis.org/A265775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 15, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   cross_lt, floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A265775
 *  a(n) = denominator q(n) of the nth upper primes‑only best
 *  approximation to sqrt(2) as defined in the OEIS comment.
 *  The function generates terms iteratively, memoising results,
 *  and returns a(n) for any 1‑based index n (in any call order).
 *
 *  The algorithm follows the definition:
 *  - p(n)/q(n) is the nth upper primes‑only best approximation to √2.
 *  - q(1)=2, p(1)=least prime ≥ √2·q(1) = 3.
 *  - For m>1: q(m) is the smallest prime q > q(m‑1) for which
 *    there exists a prime p with √2 < p/q < p(m‑1)/q(m‑1).
 *    p(m) is the least such prime p.
 *
 *  No floating‑point arithmetic is used; all comparisons are performed
 *  with exact integer arithmetic.  The code works for seq_t of any
 *  integral width (8…128 bits) or arbitrary precision.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/next_prime.h"   /* next_prime(uint64_t start) -> first prime > start */
#include "../utils/floor_sqrt.h"  /* floor_sqrt(seq_t x) -> ⌊√x⌋ */
#include "../utils/cross_lt.h"

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1U) return 0;   /* undefined for n<1 */

    /* memoised storage (1‑based, capacity data) */
    static seq_t *denom = NULL;   /* q(n) */
    static seq_t *num   = NULL;   /* p(n) */
    static size_t  cap   = 0;
    static size_t  max_n = 0;     /* highest generated index */

    /* enlarge storage if needed */
    if (n > max_n) {
        size_t need = (size_t)n + 1;          /* +1 for 1‑based indexing */
        if (need > cap) {
            size_t new_cap = cap ? cap << 1 : 32;
            if (new_cap < need) new_cap = need;
            seq_t *pt = (seq_t *)realloc(denom, new_cap * sizeof(seq_t));
            seq_t *pn = (seq_t *)realloc(num,   new_cap * sizeof(seq_t));
            if (!pt || !pn) exit(EXIT_FAILURE);
            denom = pt;
            num   = pn;
            cap   = new_cap;
        }

        /* initialise the first term if this is the first call */
        if (max_n == 0U) {
            denom[1] = (seq_t)2;          /* q(1) */
            num[1]   = (seq_t)3;          /* p(1) >= √2·q(1) */
            max_n    = 1U;
        }

        /* generate terms up to requested n */
        for (size_t m = max_n + 1U; m <= (size_t)n; ++m) {
            seq_t prev_p = num[max_n];
            seq_t prev_q = denom[max_n];

            /* next prime q > prev_q */
            uint64_t q = next_prime((uint64_t)prev_q);
            for (;; q = next_prime(q)) {
                /* compute smallest prime p satisfying √2 < p/q */
                /* use integer arithmetic: find first p such that p^2 > 2q^2 */
                seq_t two_q_sq;
#if defined(__SIZEOF_INT128__)
                unsigned __int128 tmp = (unsigned __int128)q * q;
                tmp = tmp * 2;
                two_q_sq = (seq_t)tmp;
#else
                two_q_sq = 2 * q * q;          /* safe for arbitrary‑precision seq_t */
#endif
                seq_t start = floor_sqrt(two_q_sq) + 1;   /* ceil(√2 * q) */
                if (start <= q) start = q + 1;           /* ensure p > q */

                /* first prime ≥ start */
                uint64_t p_start = next_prime((uint64_t)(start - 1));
                seq_t p = (seq_t)p_start;

                /* limit from inequality p/q < prev_p/prev_q */
                seq_t prod;
#if defined(__SIZEOF_INT128__)
                unsigned __int128 prod_tmp = (unsigned __int128)prev_p * q;
                prod = (seq_t)prod_tmp;
#else
                prod = prev_p * q;                  /* safe for arbitrary‑precision seq_t */
#endif
                seq_t limit = (prod - 1) / prev_q;   /* floor( (prev_p*q - 1)/prev_q ) */

                if (p > limit) {
                    /* no suitable p for this q; try next q */
                    continue;
                }

                /* search primes p until we satisfy p/q < prev_p/prev_q */
                while (p <= limit) {
                    if (cross_lt(p, prev_q, prev_p, q)) {   /* p/q < prev_p/prev_q */
                        denom[m] = q;
                        num[m]   = p;
                        max_n    = m;
                        goto next_term;                    /* finished this term */
                    }
                    /* next potential prime p */
                    uint64_t p_next = next_prime((uint64_t)p);
                    p = (seq_t)p_next;
                }
                /* if loop exits without break, no p found for this q:
                   try next q */
            }
        next_term:
            /* continue outer for-loop */
            ;
        }
    }

    return denom[n];
}
