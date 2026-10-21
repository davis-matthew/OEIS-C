/*
 * A289997  Numbers n whose trajectory under iteration of the map k ->
 *          (sigma(k)+phi(k))/2 never reaches a fraction (that is, either the
 *          trajectory reaches a prime, which is a fixed point, or diverges to
 *          infinity).
 * https://oeis.org/A289997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   calc_phi, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A289997.
 * An integer n belongs to the sequence iff iterating
 *
 *   k  ->  (sigma(k)+phi(k))/2
 *
 * never produces a fraction (i.e., the sum is even at every step)
 * and eventually reaches a prime (which is a fixed point).
 * The function tests consecutive integers starting from 1, keeps
 * those that satisfy the property, caches them, and returns the
 * requested term.  The caching allows calls in arbitrary order.
 */

#include <stdbool.h>
#include <stdlib.h>
/* utilities that implement sigma(n), phi(n) and primality testing */
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n);          */
#include "../utils/calc_phi.h"   /* seq_t calc_phi(seq_t n);         */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n);    */

static seq_t *a_cache     = NULL;   /* cached sequence values         */
static size_t a_cache_len = 0;      /* number of stored terms          */
static size_t a_cache_cap = 0;      /* allocated capacity              */
static idx_t  next_candidate = 1;    /* next integer to test           */

/* Test whether an integer n belongs to A289997.            */
static bool a289997_good(seq_t n)
{
    if (n == (seq_t)1)
        return true;                /* 1 is a fixed point           */

    /* Iterate the map until a prime is reached or a fraction
       appears (odd sum).  A simple step counter protects
       against accidental infinite loops.                    */
    const int MAX_STEPS = 2000;
    seq_t cur = n;
    for (int step = 0; step < MAX_STEPS; ++step) {
        seq_t sigma = sigma_of((idx_t)cur);
        seq_t phi   = calc_phi(cur);
        seq_t sum   = sigma + phi;

        /* If the sum is odd, the trajectory reaches a fraction –
           n is not in the sequence. */
        if (sum & (seq_t)1)
            return false;

        seq_t nxt = sum / 2;

        /* Reaching a prime means the trajectory eventually
           hits a fixed point. */
        if (is_prime_big(nxt))
            return true;

        /* If the map becomes periodic without reaching a prime,
           we stop.  The only non‑prime fixed point that satisfies
           the condition is 1, handled above. */
        if (nxt == cur)
            return false;

        cur = nxt;
    }

    /* If we hit the step limit, assume the trajectory diverges
       to infinity and therefore qualifies.                    */
    return true;
}

/*---------------------------------------------------------------------*/
/*  Public accessor: required signature seq_t a(idx_t n)               */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (a_cache_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (a289997_good(cand)) {
            if (a_cache_len + 1 > a_cache_cap) {
                size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* memory allocation failed */
                a_cache      = tmp;
                a_cache_cap  = new_cap;
            }
            a_cache[a_cache_len++] = cand;
        }
        ++next_candidate;
    }
    return a_cache[n - 1];
}
