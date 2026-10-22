/*
 * A302521  Odd numbers whose prime indices are squarefree and have disjoint
 *          prime indices. Numbers n such that the n-th multiset multisystem
 *          is a set partition.
 * https://oeis.org/A302521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th odd squarefree number whose prime indices are
 *            squarefree and pairwise disjoint. The function computes this by
 *            iterating over odd integers, testing the required conditions
 *            with a pre‑computed prime list
 * Range:     u8 n <= 54, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- Global data ---------- */
static uint32_t *primes = NULL;          /* primes list, 0‑based   */
static size_t   prime_cnt = 0;           /* number of primes      */
static bool     primes_built = false;    /* flag for primes ready */

static unsigned *seen = NULL;            /* tag array for disjointness check */
static unsigned seen_tag = 0;            /* epoch counter */
static bool     seen_built = false;      /* flag for seen array ready */

/* ---------- Helper functions ---------- */

/* Sieve of Eratosthenes: generate all primes up to LIMIT */
static void init_primes(void)
{
    if (primes_built) return;

    const size_t LIMIT = 1000000ULL;                /* safe upper bound */
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) return;                          /* allocation failed */

    size_t cap = LIMIT / 10 + 100;
    uint32_t *tmp = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!tmp) { free(is_comp); return; }

    size_t cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (cnt == cap) {
                cap *= 2;
                tmp = (uint32_t *)realloc(tmp, cap * sizeof(uint32_t));
                if (!tmp) break;
            }
            tmp[cnt++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (primes) {
        for (size_t i = 0; i < cnt; ++i)
            primes[i] = tmp[i];
    }
    prime_cnt = cnt;
    primes_built = true;

    free(tmp);
    free(is_comp);
}

/* Initialise tag array for disjointness test */
static void init_seen(void)
{
    if (seen_built) return;
    seen = (unsigned *)calloc(prime_cnt + 1, sizeof(unsigned));
    seen_built = true;
}

/* Binary search for prime array → returns 1‑based index or 0 if not prime */
static size_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        uint64_t val = (uint64_t)primes[mid];
        if (val == (uint64_t)p)
            return mid + 1;
        else if (val < (uint64_t)p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

/* Factor n into distinct prime factors; return true iff squarefree */
static bool factor_squarefree(uint64_t n, uint32_t *out, size_t *cnt_out)
{
    size_t c = 0;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            uint32_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                if (exp > 1) return false;    /* not squarefree */
            }
            out[c++] = (uint32_t)p;
        }
    }
    if (m > 1) out[c++] = (uint32_t)m;        /* remaining prime factor */
    *cnt_out = c;
    return true;
}

/* Test whether an odd integer satisfies the sequence conditions */
static bool valid_odd(uint64_t n)
{
    if (n == 1) return true;          /* 1 is always included */

    uint32_t fac[32];
    size_t fac_cnt;
    if (!factor_squarefree(n, fac, &fac_cnt)) return false;  /* n not squarefree */

    ++seen_tag;                       /* new epoch for this candidate */
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint32_t p = fac[i];
        size_t idx = prime_index(p);
        if (idx == 0) return false;    /* p not prime – should not happen */

        /* idx itself must be squarefree */
        uint32_t idx_fac[32];
        size_t idx_fac_cnt;
        if (!factor_squarefree(idx, idx_fac, &idx_fac_cnt))
            return false;

        /* ensure prime indices of different idx's are disjoint */
        for (size_t j = 0; j < idx_fac_cnt; ++j) {
            uint32_t q = idx_fac[j];
            if (seen[q] == seen_tag) return false;  /* conflict found */
            seen[q] = seen_tag;
        }
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for offset 0 */

    init_primes();
    init_seen();

    idx_t  count = 0;
    uint64_t candidate = 1;      /* start with odd integer 1 */
    seq_t result = 1;

    while (count < n) {
        if (valid_odd(candidate)) {
            ++count;
            result = (seq_t)candidate;
        }
        candidate += 2;          /* only odd numbers need be tested */
    }
    return result;
}
