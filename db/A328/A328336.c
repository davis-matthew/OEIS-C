/*
 * A328336  Numbers with no consecutive prime indices relatively prime.
 * https://oeis.org/A328336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer whose multiset of prime indices
 * (indices of primes dividing the number) contains no two consecutive parts
 * that are relatively prime.  The sequence is defined by enumerating such
 * integers in increasing order.  This implementation generates the sequence
 * lazily and memoises the results in a static array.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper: Euclidean GCD for small integers */
static int gcd_int(int a, int b) {
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* maximal value to sieve; chosen large enough to produce >10 000 terms. */
#define MAX_SIEVE 500000

static uint32_t *spf_arr        = NULL;  /* smallest prime factor */
static uint32_t *prime_index_arr = NULL;/* prime -> 1‑based index */
static uint32_t *primes_arr     = NULL;  /* list of primes */
static size_t    primes_cnt     = 0;
static bool      primes_init    = false;

/* initialise sieve, spf and prime index arrays */
static void init_primes(void) {
    if (primes_init) return;
    primes_init = true;

    /* allocate arrays */
    spf_arr = (uint32_t *)malloc((MAX_SIEVE + 1) * sizeof(uint32_t));
    prime_index_arr = (uint32_t *)malloc((MAX_SIEVE + 1) * sizeof(uint32_t));
    /* at most MAX_SIEVE / log(MAX_SIEVE) primes ~ 41500 for 500k */
    primes_arr = (uint32_t *)malloc((MAX_SIEVE / 10 + 10) * sizeof(uint32_t));
    if (!spf_arr || !prime_index_arr || !primes_arr) {
        /* fatal error – abort program */
        abort();
    }
    for (size_t i = 0; i <= MAX_SIEVE; ++i) {
        spf_arr[i] = 0;
        prime_index_arr[i] = 0;
    }

    primes_cnt = 0;
    for (size_t i = 2; i <= MAX_SIEVE; ++i) {
        if (spf_arr[i] == 0) {          /* i is prime */
            spf_arr[i] = (uint32_t)i;
            primes_arr[primes_cnt++] = (uint32_t)i;
            prime_index_arr[i] = (uint32_t)(primes_cnt);  /* 1‑based */
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes_arr[j];
            uint64_t x = (uint64_t)i * (uint64_t)p;
            if (x > MAX_SIEVE) break;
            if (spf_arr[x] == 0) spf_arr[x] = p;
            if (p > spf_arr[i]) break;
        }
    }
}

/* static cache of sequence values */
static seq_t  *seq_cache      = NULL;  /* values of the sequence */
static size_t seq_len         = 0;    /* number of values stored */
static size_t seq_cap         = 0;    /* capacity of seq_cache */
static idx_t  next_candidate  = 1;    /* integer to test next */

/* expand cache capacity */
static void grow_cache(void) {
    size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!seq_cache) abort();
    seq_cap = new_cap;
}

/* test whether integer n satisfies the condition */
static bool good_integer(idx_t n) {
    idx_t x = n;
    /* obtain list of prime indices (with multiplicity) */
    int indices[20];        /* 20 is enough for n <= 500000 (2^19<500k) */
    int len = 0;

    while (x > 1) {
        size_t xi = (size_t)x;
        uint32_t p = spf_arr[xi];
        int idx = (int)prime_index_arr[p];
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        for (int k = 0; k < exp; ++k) {
            indices[len++] = idx;
        }
    }

    /* sort indices in non‑increasing order */
    for (int i = 1; i < len; ++i) {
        int key = indices[i];
        int j = i - 1;
        while (j >= 0 && indices[j] < key) {
            indices[j + 1] = indices[j];
            --j;
        }
        indices[j + 1] = key;
    }

    /* check consecutive pairs for coprimality */
    for (int i = 0; i + 1 < len; ++i) {
        if (gcd_int(indices[i], indices[i + 1]) == 1)
            return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n) {
    init_primes();

    /* grow cache until we have at least n elements */
    while (seq_len < (size_t)n) {
        /* test next candidate */
        if (good_integer(next_candidate)) {
            if (seq_len == seq_cap)
                grow_cache();
            seq_cache[seq_len] = (seq_t)next_candidate;
            ++seq_len;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
