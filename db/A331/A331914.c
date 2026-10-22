/*
 * A331914  Numbers with at most one prime prime index, counted with
 *          multiplicity.
 * https://oeis.org/A331914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers with at most one prime prime index, counted with
 *            multiplicity. Computed by generating integers in order and
 *            selecting those whose multiset of prime indices has at most one
 *            element that is prime, counted with multiplicity. A linear sieve
 *            supplies prime factors and prime indices
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ----- static state ------------------------------------------------- */
static seq_t   *seq_cache   = NULL;      /* cached sequence values          */
static size_t   seq_len     = 0;          /* number of values stored         */
static size_t   seq_cap     = 0;          /* allocated capacity              */
static idx_t   *spf         = NULL;      /* smallest prime factor array     */
static idx_t     spf_limit  = 0;          /* upper bound of spf array        */
static idx_t   *prime_list  = NULL;      /* list of primes                   */
static size_t   prime_cnt   = 0;          /* number of primes                */
static size_t   prime_cap   = 0;          /* capacity of prime_list          */
static idx_t   *prime_index_of = NULL;   /* prime → 1‑based index          */
static char    *index_is_prime = NULL;   /* is an index prime?             */
static idx_t    next_candidate = 1;      /* next integer to test           */

/* ----- utility functions --------------------------------------------- */
static void init_sieve(idx_t limit) {
    /* allocate spf array */
    spf = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) abort();
    spf[0] = 0; spf[1] = 1;

    /* allocate prime list with a rough initial capacity */
    prime_cap = (size_t)(limit / 10) + 1;
    prime_list = (idx_t*)malloc(prime_cap * sizeof(idx_t));
    if (!prime_list) abort();

    /* linear sieve */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (idx_t*)realloc(prime_list, prime_cap * sizeof(idx_t));
                if (!prime_list) abort();
            }
            prime_list[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt &&
             prime_list[j] <= spf[i] &&
             i * prime_list[j] <= limit; ++j) {
            spf[i * prime_list[j]] = prime_list[j];
        }
    }

    /* prime_index_of array: index starts at 1 */
    prime_index_of = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    if (!prime_index_of) abort();
    for (size_t i = 0; i < prime_cnt; ++i) {
        prime_index_of[prime_list[i]] = (idx_t)(i + 1);
    }

    /* index_is_prime array for indices up to prime_cnt */
    index_is_prime = (char*)calloc(prime_cnt + 1, sizeof(char));
    if (!index_is_prime) abort();
    for (idx_t i = 2; i <= (idx_t)prime_cnt; ++i) {
        /* an index is prime iff spf[i] == i (since spf built up to limit) */
        index_is_prime[i] = (spf[i] == i) ? 1 : 0;
    }

    spf_limit = limit;
}

static bool is_good_number(idx_t n) {
    if (n == 1) return true;
    int count = 0;
    idx_t m = n;
    while (m > 1) {
        idx_t p = spf[m];       /* smallest prime factor */
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        idx_t idx = prime_index_of[p];           /* 1‑based index of p */
        if (idx >= 2 && index_is_prime[idx]) {
            count += e;
            if (count > 1) return false;
        }
    }
    return true;
}

static void generate_up_to(idx_t n) {
    /* Ensure the sieve covers all candidates we will test */
    if (next_candidate > spf_limit) {
        idx_t new_limit = spf_limit * 2;
        while (next_candidate > new_limit) new_limit *= 2;
        init_sieve(new_limit);
    }

    /* Grow the cache if needed */
    if (seq_cap == 0) {
        seq_cap = 32;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) abort();
    }

    while (seq_len < n) {
        if (is_good_number(next_candidate)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
                if (!seq_cache) abort();
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
        if (next_candidate > spf_limit) {
            /* increase sieve limit if we run out of precomputed spf */
            idx_t new_limit = spf_limit * 2;
            init_sieve(new_limit);
        }
    }
}

/* ----- public function ------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1 – undefined for 0 */
    static bool initialized = false;
    if (!initialized) {
        /* choose a limit that comfortably covers n up to ~10000 */
        init_sieve(500000);
        initialized = true;
    }
    if (seq_len < (size_t)n) {
        generate_up_to(n);
    }
    return seq_cache[n - 1];
}
