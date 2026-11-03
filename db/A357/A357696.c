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
 * Validated: standard, pass, 70 terms through n = 9975 (2026-11-03)
 * Helpers:   factor_u64, mem
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
#include "../utils/mem.h"

/* Recursive test: does the factorization of 'full' contain a proper
   abundant divisor?
   primes[0..cnt-1]   – prime factors of full
   exps  [0..cnt-1]   – their exponents (≤2)
   pos    – current position in the factor list
   d      – current divisor being built
   sigma_d – σ(d) (product of local sums)
*/
static bool _divisor_helper(const uint64_t *primes, const int *exps, size_t cnt,
                            uint64_t full, size_t pos, uint64_t d,
                            unsigned long long sigma_d)
{
    if (pos == cnt) {
        if (d == full) return false;            /* skip full number itself */
        unsigned long long two_d = (unsigned long long)d * 2ULL;
        return sigma_d > two_d;
    }

    uint64_t p = primes[pos];
    int      e = exps[pos];
    unsigned long long pw   = 1ULL;   /* p^k */
    unsigned long long sum  = 1ULL;   /* sum_{t=0}^k p^t */

    for (int k = 0; k <= e; ++k) {
        if (k > 0) {
            pw   *= (unsigned long long)p;
            sum  += pw;
        }
        uint64_t new_d   = d * pw;
        unsigned long long new_sigma = sigma_d * sum;
        if (_divisor_helper(primes, exps, cnt, full, pos + 1,
                            new_d, new_sigma))
            return true;
    }
    return false;
}

/* Wrapper that initiates the recursion. */
static bool has_abundant_divisor(const uint64_t *primes, const int *exps,
                                 size_t cnt, uint64_t full)
{
    return _divisor_helper(primes, exps, cnt, full, 0, 1ULL, 1ULL);
}

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
            seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
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
