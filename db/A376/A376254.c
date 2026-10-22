/*
 * A376254  Numbers k such that A376294(k) < k.
 * https://oeis.org/A376254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that A376294(k) < k,
 *            where A376294(k) is the product of the decimal concatenations of
 *            each prime factor base and its exponent in the factorization of
 *            k. The implementation searches integers incrementally, memoises
 *            the qualifying numbers and returns the requ...
 * Range:     u8 n <= 1, u16 n <= 363, u32 n <= 9869, u64 n <= 9869,
 *            u128 n <= 9869, big any n
 * Helpers:   build_primes_upto, digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Required utility headers  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
/*  Functionality assumed to exist in the build environment  */
#include "../utils/pow10.h"          /* returns seq_t 10^e   */
#include "../utils/digits_of.h"      /* returns number of digits of an integer */
#include "../utils/build_primes_upto.h"   /* fills prime list up to a limit */

/*  Prime list  */
static uint64_t *primes      = NULL;   /* array of primes */
static size_t   primes_cnt   = 0;      /* number of primes */

/*  Initialise the prime list once.  */
static void init_primes(void)
{
    if (primes_cnt != 0) {
        return;
    }
    /*  A fixed limit large enough to factor all numbers needed
        for the first 10 000 terms.  This is roughly 2×10⁶.  */
    const idx_t limit = 2000000ULL;
    build_primes_upto(limit, &primes, &primes_cnt);
}

/*  Return true iff k satisfies A376294(k) < k.  */
static bool qualifies(uint64_t k)
{
    seq_t prod = 1;               /* multiplicative product of concatenations */
    uint64_t n  = k;              /* remaining part of k to factor */
    seq_t k_seq = (seq_t)k;       /* cast once for comparisons */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > n) {
            break;
        }
        if (n % p != 0) {
            continue;
        }
        uint64_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        /*  Concatenate p and exp: p * 10^digits(exp) + exp.  */
        seq_t part = ((seq_t)p) * pow10((idx_t)digits_of(exp))
                       + (seq_t)exp;
        /*  Avoid overflow: if prod * part ≥ k break early.  */
        if (prod > k_seq / part) {
            return false;
        }
        prod *= part;
    }

    /*  Remaining prime factor (if any), exponent 1.  */
    if (n > 1) {
        seq_t part = ((seq_t)n) * pow10((idx_t)digits_of(1))
                       + (seq_t)1;
        if (prod > k_seq / part) {
            return false;
        }
        prod *= part;
    }

    return prod < k_seq;
}

/*  Cache of qualifying numbers  */
static seq_t *cache      = NULL;   /* array of qualifying numbers */
static size_t cache_size = 0;      /* number of stored terms       */
static size_t cache_cap  = 0;      /* capacity of cache array      */
static uint64_t last_checked = 0;  /* last integer examined        */

/*  Grow the cache array when needed.  */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort(); /* allocation failure → terminate */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/*  Public API  */
seq_t a(idx_t n)
{
    if (n == 0) {                 /* offset 1; guard against 0 */
        return 0;
    }

    init_primes();

    /*  Generate terms until we have at least n of them.  */
    while (cache_size < (size_t)n) {
        ++last_checked;
        if (qualifies(last_checked)) {
            if (cache_size == cache_cap) {
                grow_cache();
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
