/*
 * A349794  Numbers whose prime signature has an odd term other than the first
 *          or last.
 * https://oeis.org/A349794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest integer whose prime signature *
 *            (sequence of exponents in its prime factorisation) contains an
 *            odd * exponent in some interior position (i.e., not the first or
 *            last). * Computed by incremental search with trial division
 *            using an * array of smallest prime factors
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"

static seq_t *seq_cache    = NULL;   /* stored a(1), a(2), …   */
static size_t seq_capacity = 0;      /* size of seq_cache      */
static size_t seq_size     = 0;      /* number of generated terms */

static idx_t *spf         = NULL;   /* array of smallest prime factors */
static idx_t spf_limit    = 0;       /* maximum n for which spf is defined */
static idx_t candidate    = 2;       /* next integer to test             */

/* Ensure we have smallest prime factors up to at least 'up_to'. */
static void ensure_spf(idx_t up_to)
{
    if (up_to <= spf_limit)
        return;
    idx_t new_limit = spf_limit > 0 ? spf_limit * 2 : up_to;
    if (new_limit < up_to)
        new_limit = up_to;
    if (spf)
        free(spf);
    build_spf(new_limit, &spf);
    spf_limit = new_limit;
}

/* Return true iff n has at least three distinct prime factors and
 * an odd exponent among the interior primes. */
static bool qualifies(idx_t n)
{
    idx_t tmp = n;
    int exps[64];
    int distinct = 0;

    while (tmp > 1) {
        idx_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        exps[distinct++] = e;
    }
    if (distinct < 3)
        return false;
    for (int i = 1; i < distinct - 1; ++i) {
        if (exps[i] & 1)
            return true;
    }
    return false;
}

/* Generate terms up to and including the 'target'-th term. */
static void generate_terms(idx_t target)
{
    while (seq_size < (size_t)target) {
        if (candidate > spf_limit)
            ensure_spf(candidate * 2 + 1000);

        if (qualifies(candidate)) {
            if (seq_size >= seq_capacity) {
                size_t new_cap = seq_capacity ? seq_capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure – abort silently */
                    return;
                seq_cache    = tmp;
                seq_capacity = new_cap;
            }
            seq_cache[seq_size++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* Public sequence entry function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0 – return 0 */
    generate_terms(n);
    return seq_cache[n - 1];
}
