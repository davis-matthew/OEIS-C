/*
 * A343013  Lexicographically earliest strictly increasing sequence of numbers
 *          whose partial products have mutually distinct exponents in their
 *          prime factorization (A130091).
 * https://oeis.org/A343013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1, a(n) is the smallest integer > a(n‑1) such
 *            that, if P = a(1)*…*a(n‑1), the prime‑factorization of P·a(n)
 *            has all exponents pairwise distinct. The function builds the
 *            sequence lazily and is independent of the order of calls
 * Range:     u8 n <= 104, u16 n <= 9615, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exponents_all_distinct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "../utils/exponents_all_distinct.h"

/* ======================================================================
   Prime sieve (up to 2 million) – enough for n ≤ 10 000.
   ====================================================================== */
#define PRIME_SIEVE_LIMIT 2000000
static uint32_t *primes = NULL;      /* array of primes ≤ PRIME_SIEVE_LIMIT */
static size_t prime_count = 0;       /* number of primes stored */

static void init_primes(void)
{
    if (primes != NULL) return;          /* already initialized */
    bool *is_comp = (bool *)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!is_comp) { abort(); }
    memset(is_comp, 0, PRIME_SIEVE_LIMIT + 1);
    primes = (uint32_t *)malloc((PRIME_SIEVE_LIMIT / 10) * sizeof(uint32_t));
    if (!primes) { abort(); }
    for (uint32_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            primes[prime_count++] = i;
    free(is_comp);
}

/* factor_uint64: factorize n into primes stored in primes[].
   Output arrays out_primes / out_exps are filled, *out_count set.
   The arrays must be large enough (e.g. 128).                       */
static void factor_uint64(uint64_t n,
                          uint32_t out_primes[],
                          uint32_t out_exps[],
                          size_t *out_count)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            out_primes[cnt] = p;
            out_exps[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        out_primes[cnt] = (uint32_t)tmp;
        out_exps[cnt] = 1;
        ++cnt;
    }
    *out_count = cnt;
}

/* ======================================================================
   Sequence generation.
   ====================================================================== */
#define INITIAL_CACHE_CAP 16
#define TEMP_EXPS_CAP 256

/* dynamic cache of computed values */
static seq_t *seq_cache = NULL;     /* array of a(i) */
static idx_t seq_cache_len = 0;     /* number of terms stored */
static size_t seq_cache_cap = 0;    /* allocated size */

/* current product exponents – stored as parallel arrays.
   cur_primes[k] => prime p
   cur_exps[k]   => exponent of p in current product
   cur_prime_count: number of distinct primes that appear.          */
#define CUR_PRIME_CAP 256
static uint32_t cur_primes[CUR_PRIME_CAP];
static uint32_t cur_exps[CUR_PRIME_CAP];
static size_t cur_prime_count = 0;

/* ----------------------------------------------------------------------
   helpers
   ---------------------------------------------------------------------- */

/* append value to seq_cache, expanding if needed */
static void seq_cache_append(seq_t value)
{
    if (seq_cache_len == seq_cache_cap) {
        size_t new_cap = seq_cache_cap == 0 ? INITIAL_CACHE_CAP : seq_cache_cap * 2;
        if (new_cap < seq_cache_len + 1) new_cap = seq_cache_len + 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache_cap = new_cap;
    }
    seq_cache[seq_cache_len++] = value;
}

/* ----------------------------------------------------------------------
   main function
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for non‑positive n */

    /* ensure all helper tables are ready */
    init_primes();

    /* generate terms up to n lazily */
    while (seq_cache_len < n) {

        if (seq_cache_len == 0) {   /* first term a(1) = 1   */
            seq_t first = 1;
            seq_cache_append(first);
            /* initial product has no prime factors – exponents empty */
            cur_prime_count = 0;
            continue;
        }

        /* start searching for the next term */
        seq_t prev = seq_cache[seq_cache_len - 1];
        seq_t cand = prev + 1;

        for (;;) {
            /* factorization: cand is assumed to fit in uint64_t */
            uint64_t cand_u = (uint64_t)cand;
            uint32_t fac_primes[128];
            uint32_t fac_exps[128];
            size_t fac_cnt;
            factor_uint64(cand_u, fac_primes, fac_exps, &fac_cnt);

            /* copy current exponents to temporary storage */
            uint32_t temp_exps[CUR_PRIME_CAP];
            memcpy(temp_exps, cur_exps, cur_prime_count * sizeof(uint32_t));
            size_t temp_count = cur_prime_count;
            bool overflow = false;

            /* update exponents by adding factorization of cand */
            for (size_t i = 0; i < fac_cnt && !overflow; ++i) {
                uint32_t p = fac_primes[i];
                uint32_t e = fac_exps[i];

                /* search for prime p in current list */
                size_t j;
                for (j = 0; j < cur_prime_count; ++j)
                    if (cur_primes[j] == p) break;

                if (j < cur_prime_count) {
                    /* existing prime – increase exponent */
                    temp_exps[j] += e;
                } else {
                    /* new prime – append if capacity allows */
                    if (temp_count >= CUR_PRIME_CAP) {
                        overflow = true;
                        break;
                    }
                    cur_primes[temp_count] = p;   /* store for next step */
                    temp_exps[temp_count] = e;
                    ++temp_count;
                }
            }

            if (overflow) {
                /* number is too large for our fixed arrays – skip */
                cand++;
                continue;
            }

            /* sort the temporary exponents to check uniqueness */
            /* simple insertion sort – small array */
            for (size_t i = 1; i < temp_count; ++i) {
                uint32_t key = temp_exps[i];
                size_t j = i;
                while (j > 0 && temp_exps[j - 1] > key) {
                    temp_exps[j] = temp_exps[j - 1];
                    --j;
                }
                temp_exps[j] = key;
            }

            if (exponents_all_distinct(temp_exps, temp_count)) {
                /* candidate accepted */
                seq_cache_append(cand);

                /* update the real current primes/exponents arrays */
                for (size_t i = 0; i < fac_cnt; ++i) {
                    uint32_t p = fac_primes[i];
                    uint32_t e = fac_exps[i];
                    size_t j;
                    for (j = 0; j < cur_prime_count; ++j)
                        if (cur_primes[j] == p) break;
                    if (j < cur_prime_count) {
                        cur_exps[j] += e;
                    } else {
                        cur_primes[cur_prime_count] = p;
                        cur_exps[cur_prime_count] = e;
                        ++cur_prime_count;
                    }
                }
                break;
            }

            /* otherwise try next candidate */
            cand++;
        }
    }

    return seq_cache[n - 1];
}
