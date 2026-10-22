/*
 * A327476  Heinz numbers of integer partitions whose mean A326567/A326568 is
 *          not a part.
 * https://oeis.org/A327476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th Heinz number of an integer partition whose mean
 *            part size (sum of parts / number of parts) is not equal to any
 *            part in the partition. The sequence starts at a(1) = 1 (empty
 *            partition) and continues per the OEIS listing
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*           global tables for the linear sieve                       */
/* ------------------------------------------------------------------ */
static idx_t *spf          = NULL;   /* smallest prime factor of i   */
static idx_t *prime_idx    = NULL;   /* 1‑based index of prime i     */
static idx_t  sieve_limit  = 0;      /* currently sieved limit       */
static idx_t *primes       = NULL;   /* list of primes generated    */
static size_t primes_cap   = 0;      /* capacity of primes array     */
static size_t primes_cnt   = 0;      /* number of primes generated   */

/* ------------------------------------------------------------------ */
/*   global cache for the computed terms of the sequence             */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache   = NULL;   /* cache of sequence values     */
static size_t cache_cap   = 0;      /* capacity of the cache        */
static size_t cache_size  = 0;      /* number of terms stored       */
static idx_t  next_test   = 2;      /* next integer to test         */

/* ------------------------------------------------------------------ */
/* helper: make sure seq_cache has room for at least `need` slots     */
/* ------------------------------------------------------------------ */
static void ensure_seq_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* helper: linear sieve up to `limit` (re‑generates entire tables)    */
/* ------------------------------------------------------------------ */
static void extend_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* allocate new arrays of the new limit */
    idx_t *new_spf      = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    idx_t *new_prime_idx= (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    /* initialise to zero */
    for (idx_t i = 0; i <= limit; ++i) {
        new_spf[i] = 0;
        new_prime_idx[i] = 0;
    }

    /* prime list */
    size_t cap = limit / 10 + 10;
    idx_t *new_primes = (idx_t *)malloc(cap * sizeof(idx_t));
    size_t cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (cnt == cap) {
                cap <<= 1;
                new_primes = (idx_t *)realloc(new_primes, cap * sizeof(idx_t));
            }
            new_primes[cnt++] = i;
            new_prime_idx[i] = (idx_t)cnt;   /* 1‑based index of the prime */
        }
        for (size_t j = 0; j < cnt; ++j) {
            idx_t p = new_primes[j];
            if (p > new_spf[i] || i * p > limit) break;
            new_spf[i * p] = p;
        }
    }

    /* free old tables */
    free(spf);
    free(prime_idx);
    free(primes);

    /* hand over new tables */
    spf        = new_spf;
    prime_idx  = new_prime_idx;
    primes     = new_primes;
    primes_cap = cap;
    primes_cnt = cnt;
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* test whether integer m satisfies the mean condition               */
/* ------------------------------------------------------------------ */
static bool is_valid(idx_t m)
{
    if (m == 1) return true;          /* empty partition */

    idx_t tmp = m;
    idx_t k = 0;   /* total number of parts (total multiplicity) */
    idx_t S = 0;   /* sum of part indices weighted by multiplicity */

    /* local storage for distinct factor indices & exponents */
    idx_t idx_list[64];
    idx_t exp_list[64];
    size_t uniq = 0;

    while (tmp > 1) {
        idx_t p = spf[tmp];
        idx_t j = prime_idx[p];   /* prime index of p (1‑based) */
        idx_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        idx_list[uniq]   = j;
        exp_list[uniq]   = e;
        k += e;
        S += j * e;
        ++uniq;
    }

    /* k > 0 here, m > 1 */
    if (S % k == 0) {
        idx_t mean = S / k;
        for (size_t t = 0; t < uniq; ++t)
            if (idx_list[t] == mean) return false;  /* mean equals a part */
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_size == 0) {
        ensure_seq_cache(8);
        seq_cache[0] = (seq_t)1;    /* a(1) = 1 */
        cache_size  = 1;
        next_test   = 2;
    }

    /* generate terms until we have at least n entries */
    while (cache_size < n) {
        for (idx_t m = next_test; ; ++m) {
            extend_sieve(m);
            if (is_valid(m)) {
                ensure_seq_cache(cache_size + 1);
                seq_cache[cache_size++] = (seq_t)m;
                if (cache_size >= n) {
                    next_test = m + 1;
                    break;
                }
            }
        }
    }

    return seq_cache[n - 1];
}
