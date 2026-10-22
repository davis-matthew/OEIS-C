/*
 * A357696  Cubefree primitive abundant numbers: cubefree abundant numbers
 *          having no abundant proper divisor.
 * https://oeis.org/A357696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1839, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _divisor_helper, factor_u64, has_abundant_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n‑th cubefree primitive abundant number.
    A number is primitive abundant if it is
        * abundant: σ(n) > 2n
        * cubefree: every prime exponent ≤ 2
        * no proper divisor is abundant.
    The function generates positive integers sequentially, filters them
    by the above criteria, and caches the results so that calls in any
    order are handled efficiently.  The cache grows as needed.
    (The algorithm is O(M·π(M)) where M is the largest searched
    integer; this is fast enough for n ≤ 10 000 on the test sets.) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/_divisor_helper.h"
#include "../utils/has_abundant_divisor.h"

/* -----------  helpers  ------------------------------------------- */

/* -------------------------------------------------------------- */
/*  Main predicate: cubefree, abundant and primitive              */
static bool is_cubefree_primitive_abundant(uint64_t n)
{
    if (n < 12) return false;          /* first known number */

    /* factor n --------------------------------------------------- */
    uint64_t primes[20];
    uint64_t expsU64[20];
    size_t   fac_cnt = 0;
    factor_u64(n, primes, expsU64, &fac_cnt);

    /* convert exponents to int ---------------------------------- */
    int exps[20];
    for (size_t i = 0; i < fac_cnt; ++i) exps[i] = (int)expsU64[i];

    /* cubefree test ------------------------------------------------*/
    for (size_t i = 0; i < fac_cnt; ++i)
        if (exps[i] >= 3) return false;

    /* compute σ(n) -------------------------------------------------*/
    unsigned long long sigma = 1ULL;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = primes[i];
        int      e = exps[i];
        unsigned long long pw  = 1ULL;
        unsigned long long sum = 1ULL;
        for (int k = 0; k <= e; ++k) {
            if (k > 0) {
                pw  *= (unsigned long long)p;
                sum += pw;
            }
        }
        sigma *= sum;
    }

    /* abundance test ----------------------------------------------*/
    unsigned long long two_n = (unsigned long long)n * 2ULL;
    if (sigma <= two_n) return false;

    /* primitive test ----------------------------------------------*/
    if (has_abundant_divisor(primes, exps, fac_cnt, n))
        return false;

    return true;
}

/* -----------  cache for the sequence ----------------------------- */
static seq_t *seq_cache = NULL;   /* array of found terms */
static size_t seq_len  = 0;       /* number of stored terms  */
static size_t seq_cap  = 0;       /* allocated capacity       */

/* -------------------------------------------------------------- */
/*  The exported function a(idx_t n)                              */
seq_t a(idx_t n)
{
    /* grow the cache until it contains n terms */
    while (seq_len < (size_t)n) {
        uint64_t candidate = 0;
        /* first candidate after the last stored value */
        if (seq_len > 0)
            candidate = (uint64_t)seq_cache[seq_len - 1];
        else
            candidate = 0;

        /* find next suitable number */
        do {
            ++candidate;
        } while (!is_cubefree_primitive_abundant(candidate));

        /* ensure capacity */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* memory allocation failure – return 0 as a sentinel */
                return (seq_t)0;
            }
            seq_cache = tmp;
            seq_cap   = new_cap;
        }

        seq_cache[seq_len++] = (seq_t)candidate;
    }
    return seq_cache[n - 1];
}
