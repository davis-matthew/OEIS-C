/*
 * A343338  Numbers with no prime index dividing or divisible by all the other
 *          prime indices.
 * https://oeis.org/A343338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A343338: Intersection of A342193 and A343337 */
/* This implementation generates the sequence on demand by
   maintaining a smallest‑prime‑factor table up to the current
   candidate and testing the required property.  The algorithm is
   essentially an O(n log n) sieve with a modest constant factor
   and satisfies the time limits for n ≈ 10 000. */

#include <stdlib.h>
#include <stdbool.h>

/* ----------------- cache and tables ----------------- */

static uint32_t *spf          = NULL;   /* smallest prime factor array (1‑based) */
static size_t   spf_cap       = 0;      /* current size of spf - 1 (max index protected) */
static uint32_t *primes       = NULL;   /* list of primes up to spf_cap */
static size_t   primes_cnt    = 0;      /* number of stored primes */

static seq_t   *seq_cache     = NULL;   /* cached sequence values */
static size_t   cache_len     = 0;      /* number of entries already cached */
static idx_t    next_candidate = 1;     /* next integer to test */

/* ----------------- helper functions ----------------- */

/* Extend the spf table and rebuild the prime list up to 'limit' (inclusive). */
static void ensure_spf(idx_t limit) {
    if ((idx_t)spf_cap >= limit) return;

    /* enlarge the spf array */
    size_t new_cap = (size_t)limit + 1;          /* indices start at 0 (unused) */
    uint32_t *new_spf = (uint32_t *)realloc(spf, new_cap * sizeof(uint32_t));
    if (!new_spf) exit(1);
    for (size_t i = spf_cap; i < new_cap; ++i) new_spf[i] = (uint32_t)i;

    /* sieve smallest prime factor for the new range */
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (new_spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= (size_t)limit; j += i) {
                if (new_spf[j] == (uint32_t)j) new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* rebuild prime list */
    size_t count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (new_spf[i] == (uint32_t)i) ++count;

    uint32_t *new_primes = (uint32_t *)realloc(primes, count * sizeof(uint32_t));
    if (!new_primes) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (new_spf[i] == (uint32_t)i) new_primes[idx++] = (uint32_t)i;

    /* commit changes */
    spf       = new_spf;
    spf_cap   = (size_t)limit;
    primes    = new_primes;
    primes_cnt = count;
}

/* Return 1‑based index of prime 'p' in the prime list. */
static size_t prime_index(uint32_t p) {
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] == p) return mid + 1;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0;   /* should never happen for a prime */
}

/* Test whether integer 'm' satisfies the sequence property. */
static bool is_candidate_valid(idx_t m) {
    if (m == 1) return true;                /* 1 has an empty set of indices */

    ensure_spf(m);

    /* Factor m and collect the indices of its prime factors, with multiplicities. */
    uint32_t indices[32];
    size_t   L = 0;
    idx_t tmp = m;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        size_t cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        size_t idx = prime_index(p);
        for (size_t t = 0; t < cnt && L < 32; ++t) indices[L++] = (uint32_t)idx;
    }

    /* Property: no index divides every other index, and no index is
       divisible by every other index. */
    for (size_t i = 0; i < L; ++i) {
        uint32_t a = indices[i];
        bool divides_all = true;
        bool divisible_by_all = true;
        for (size_t j = 0; j < L; ++j) {
            if (i == j) continue;
            uint32_t b = indices[j];
            if (b % a != 0) divides_all = false;
            if (a % b != 0) divisible_by_all = false;
            if (!divides_all && !divisible_by_all) break;
        }
        if (divides_all || divisible_by_all) return false;
    }
    return true;
}

/* ----------------- public function ----------------- */

seq_t a(idx_t n) {
    while (cache_len < n) {
        if (is_candidate_valid(next_candidate)) {
            seq_t *tmp = (seq_t *)realloc(seq_cache, (cache_len + 1) * sizeof(seq_t));
            if (!tmp) exit(1);
            seq_cache = tmp;
            seq_cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
