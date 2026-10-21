/*
 * A298120  Matula-Goebel numbers of rooted trees in which all positive
 *          outdegrees are odd.
 * https://oeis.org/A298120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth Matula‑Goebel number of a rooted tree in which
 *            all positive outdegrees are odd (OEIS A298120)
 * Range:     u8 n <= 58, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const idx_t LIMIT = 5000000;          /* maximum value factorized by the sieve */

static uint32_t *spf          = NULL;    /* smallest prime factor */
static uint32_t *prime_index  = NULL;    /* prime → 1‑based index (0 if not prime) */
static uint8_t  *in_seq       = NULL;    /* in_seq[x] == 1 ⇔ x is in sequence */
static seq_t   *seq          = NULL;    /* dynamic array of sequence values */
static size_t   seq_len      = 0;        /* number of terms computed */
static size_t   seq_cap      = 0;        /* capacity of seq array */
static idx_t    next_candidate = 2;      /* next integer to test for membership */
static bool     initialized   = false;   /* one‑time flag */

/* Build sieve up to LIMIT, filling spf, prime_index, and a list of primes. */
static void init_sieve(void) {
    /* Allocate arrays. C++ needs casts from void*. */
    spf         = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    prime_index = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    in_seq      = (uint8_t*) calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    if (!spf || !prime_index || !in_seq) exit(EXIT_FAILURE);

    /* Prime sieve and fill spf. */
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = (uint32_t)i;
    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {           /* i is prime */
            ++prime_cnt;
            prime_index[i] = (uint32_t)prime_cnt;
            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* Initialize sequence array with a(1)=1. */
    seq_cap = 1024;
    seq     = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);
    seq[0] = (seq_t)1;
    seq_len = 1;
    in_seq[1] = 1;   /* 1 is in the sequence */
}

/* Ensure capacity of seq. */
static void ensure_seq_capacity(void) {
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap << 1;
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq     = tmp;
        seq_cap = new_cap;
    }
}

/* Test whether integer m belongs to the sequence. */
static bool is_valid_member(idx_t m) {
    idx_t tmp = m;
    idx_t total_factors = 0;

    /* Factor m using spf up to LIMIT. */
    while (tmp > 1) {
        uint32_t p = spf[(size_t)tmp];
        if (p == 0) p = (uint32_t)tmp;           /* safety */
        uint32_t exp = 0;
        while (tmp % p == 0) {
            tmp   /= p;
            ++exp;
            ++total_factors;
        }
        uint32_t idx_p = prime_index[p];
        if (idx_p == 0 || !in_seq[idx_p]) return false; /* prime index not in sequence */
    }
    return (total_factors & 1U) == 1U;   /* odd number of prime factors */
}

/* Generate sequence up to target index. */
static void generate_until(idx_t target) {
    while (seq_len < target) {
        while (!is_valid_member(next_candidate)) ++next_candidate;
        /* next_candidate is a new member. */
        ensure_seq_capacity();
        seq[seq_len] = (seq_t)next_candidate;
        ++seq_len;
        if (next_candidate <= LIMIT) in_seq[next_candidate] = 1;
        ++next_candidate;
    }
}

/* External function required by the build. */
seq_t a(idx_t n) {
    if (!initialized) {
        init_sieve();
        initialized = true;
    }
    if (n == 0) return (seq_t)0;          /* undefined for n<1 */
    generate_until(n);
    return seq[n - 1];
}
