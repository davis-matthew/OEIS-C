/*
 * A365249  Composite numbers k for which A214749(k) = (k-1)/2.
 * https://oeis.org/A365249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 3330, u32 n <= 6762, u64 n <= 10000,
 *            u128 n <= 6762, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function generates the OEIS sequence A365249.
   a(n) = the n-th odd composite k such that the smallest
          m with (k−m) | (k²+m) equals (k−1)/2.
   The computation checks, for each odd composite k,
   whether the maximum proper divisor d of k(k+1) that is
   < k is exactly (k+1)/2.  If so, k is a term of the
   sequence.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

/* compare function for qsort (uint64_t ascending) */
static int cmp_u64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* generate all divisors of a number given its prime factorisation */
static void gen_divisors(const uint64_t *pr, const uint64_t *exp,
                         size_t cnt, uint64_t *divs, size_t *out_cnt)
{
    size_t len = 1;
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t e   = exp[i];
        size_t old_len = len;
        for (size_t j = 0; j < old_len; ++j) {
            uint64_t base = divs[j];
            for (uint64_t k = 1; k <= e; ++k) {
                base *= p;
                divs[len++] = base;
            }
        }
    }
    *out_cnt = len;
}

/*  returns true iff the odd composite k satisfies the condition   */
static bool a365249_term(uint64_t k)
{
    /* factor k */
    const size_t MAXF = 64;                  /* enough for all k <= 2^64   */
    uint64_t primes_k[MAXF];
    uint64_t exps_k[MAXF];
    size_t cnt_k = 0;
    factor_u64(k, primes_k, exps_k, &cnt_k);

    /* factor k+1 */
    uint64_t primes_k1[MAXF];
    uint64_t exps_k1[MAXF];
    size_t cnt_k1 = 0;
    factor_u64(k + 1, primes_k1, exps_k1, &cnt_k1);

    /* generate divisors of k and k+1 */
    /*  estimate maximum divisor count ~ 2000 for numbers < 1e6   */
    static uint64_t divs_k[2000];
    static uint64_t divs_k1[2000];
    size_t dcnt_k = 0, dcnt_k1 = 0;

    gen_divisors(primes_k, exps_k, cnt_k, divs_k, &dcnt_k);
    gen_divisors(primes_k1, exps_k1, cnt_k1, divs_k1, &dcnt_k1);

    qsort(divs_k, dcnt_k, sizeof(uint64_t), cmp_u64);
    qsort(divs_k1, dcnt_k1, sizeof(uint64_t), cmp_u64);

    /* find the maximum divisor d < k of k(k+1) */
    uint64_t d_max = 0;
    for (size_t i = 0; i < dcnt_k; ++i) {
        uint64_t d1 = divs_k[i];
        if (d1 >= k) break;                 /* unlikely, but safe */
        for (size_t j = 0; j < dcnt_k1; ++j) {
            uint64_t d2 = divs_k1[j];
            uint64_t prod;
            if (d2 == 0) continue;
            unsigned __int128 tmp = (unsigned __int128)d1 * d2;
            if (tmp >= k) break;             /* d2 ascending, further will be larger */
            prod = (uint64_t)tmp;
            if (prod > d_max) d_max = prod;
        }
    }

    uint64_t target = (k + 1ULL) / 2ULL;
    return d_max == target;
}

/* sequence storage and construction ------------------------------- */

static seq_t *seq_cache = NULL;      /* cached sequence values   */
static size_t cache_len   = 0;       /* number stored so far     */
static size_t cache_cap   = 0;       /* capacity of seq_cache    */
static uint64_t next_k    = 3;       /* next odd number to test  */

/* enlarge cache when needed ------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    /* grow cache capacity if needed */
    if (cache_len == 0) {
        cache_cap = 64;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
    }

    /* start searching from the currently known odd composite */
    while (cache_len < (size_t)n) {
        /* skip even numbers */
        while ((next_k & 1ULL) == 0) ++next_k;

        /* skip primes and numbers <=1 */
        if (next_k <= 1 || is_prime(next_k)) {
            ++next_k;
            continue;
        }

        /* check the condition */
        if (a365249_term(next_k)) {
            seq_cache[cache_len++] = (seq_t)next_k;
        }

        /* move to next odd composite */
        next_k += 2;
    }
}

/* public function required by the build ------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1 */
    ensure_cache(n);
    return seq_cache[n - 1];
}
