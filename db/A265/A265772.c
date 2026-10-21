/*
 * A265772  Numerators of lower primes-only best approximates (POBAs) to
 *          sqrt(2); see Comments.
 * https://oeis.org/A265772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 14, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   floor_sqrt, get_prime, is_prime_big, seq_cmp, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function generates the OEIS sequence A265772: the numerators of the
 * lower primes‑only best approximations to √2.  The algorithm processes
 * successive prime denominators q, computes the largest prime numerator
 * p ≤ ⌊q·√2⌋, and records the pair (p,q) whenever the fraction p/q is
 * strictly larger than all previously found fractions.  Those recorded
 * numerators form the sequence.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_prime.h"     /* get_prime(idx_t) -> uint32_t */
#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t) -> bool */
#include "../utils/floor_sqrt.h"    /* floor_sqrt(seq_t) -> seq_t */
#include "../utils/seq_mul.h"       /* seq_mul(seq_t,seq_t) -> seq_t */
#include "../utils/seq_cmp.h"       /* seq_cmp(const void*,const void*) -> int */

/* memoisation storage -----------------------------------------------------*/
static seq_t *cache      = NULL;  /* cache of numerators */
static size_t cache_len  = 0;     /* number of stored terms */
static size_t cache_cap  = 0;     /* allocated capacity */

/* tracking best fraction found so far -------------------------------------*/
static seq_t best_num = 0; /* numerator of best fraction seen so far */
static seq_t best_den = 1; /* denominator of best fraction seen so far */

/* index of next prime denominator to examine -----------------------------*/
static idx_t next_prime_idx = 1;  /* prime #1 is 2 */

/* Compare two fractions p/q and best_num/best_den without overflow.        */
static bool better_fraction(seq_t p, seq_t q)
{
    seq_t lhs = seq_mul(p, best_den);      /* p * best_den     */
    seq_t rhs = seq_mul(best_num, q);      /* best_num * q   */
    return seq_cmp(&lhs, &rhs) > 0;        /* lhs > rhs ? */
}

/* ------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range; harmless */

    /* Return cached value if already computed. */
    if (n <= cache_len) return cache[n - 1];

    /* Compute terms until we have at least n cached values. */
    while (cache_len < n) {
        /* ----- fetch next prime denominator ---------------------------- */
        uint32_t prime_q = get_prime(next_prime_idx++);
        seq_t   q = (seq_t)prime_q;

        /* ----- find largest prime p ≤ floor(q·√2) --------------------- */
        seq_t two_qq = seq_mul((seq_t)2, seq_mul(q, q));
        seq_t L      = floor_sqrt(two_qq);        /* L = ⌊q·√2⌋           */
        seq_t p = L;
        while (!is_prime_big(p)) --p;

        /* ----- record if new fraction is better ------------------------ */
        if (better_fraction(p, q)) {
            best_num = p;
            best_den = q;

            /* grow cache when necessary */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = p;
        }
    }

    return cache[n - 1];
}
