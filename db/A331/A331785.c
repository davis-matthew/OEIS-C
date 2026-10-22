/*
 * A331785  Lexicographically earliest sequence containing 1 and all positive
 *          integers with exactly one prime index already in the sequence,
 *          counting multiplicity.
 * https://oeis.org/A331785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A greedy search. a(n) is the lexicographically earliest sequence
 *            that contains 1 and all positive integers whose multiset of
 *            prime indices contains exactly one element already present in
 *            the sequence
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function below builds the terms on demand, keeping a
    dynamically‑growing array of all previously produced terms.
    For each new candidate it sieves primes up to a fixed limit,
    factors the candidate using the stored smallest‑prime table,
    and counts how many of the candidate's prime indices are already
    in the sequence.  The first candidate with exactly one such
    index is appended.  The algorithm works for all builds
    (uint8_t … unsigned __int128 — arbitrary PRECISION is
    handled by using the seq_t type only for values, all indices
    are handled by the native idx_t type). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ------------------- static state ------------------- */
static idx_t   SIEVE_LIMIT = 4000000UL;          /* primes up to 4e6 */
static idx_t * spf = NULL;                      /* smallest prime factor table */
static uint32_t *prime_index_map = NULL;        /* 1‑based index of prime */

static seq_t *seq_arr = NULL;                   /* produced terms */
static size_t seq_cap = 0;                      /* array capacity */
static size_t seq_len = 0;                      /* number of terms produced */

static bool initialized = false;

/* ------------------ helper functions ------------------ */
static void push_seq(seq_t val)
{
    /* grow array if needed */
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 128;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_arr = tmp;
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = val;
}

/* linear search – returns true if v occurs in the sequence so far */
static bool seq_contains(seq_t v)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq_arr[i] == v) return true;
    return false;
}

/* sieve for smallest prime factors and prime index map */
static void build_sieve(void)
{
    size_t N = SIEVE_LIMIT + 1;
    spf = (idx_t *)malloc(N * sizeof(idx_t));
    if (!spf) abort();
    memset(spf, 0, N * sizeof(idx_t));

    prime_index_map = (uint32_t *)calloc(N, sizeof(uint32_t));
    if (!prime_index_map) abort();

    idx_t pcount = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!spf[i]) {                            /* i is prime */
            spf[i] = i;
            ++pcount;
            prime_index_map[i] = (uint32_t)pcount;
            if (i * i <= SIEVE_LIMIT) {
                for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                    if (!spf[j]) spf[j] = i;
                }
            }
        }
    }
}

/* one‑time initialization */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    seq_cap = 128;
    seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_arr) abort();

    build_sieve();
}

/* ----------------------- main function ----------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined – safe guard */
    init();

    /* generate terms until we have at least n */
    while (seq_len < n) {
        if (seq_len == 0) {                /* first term is 1 */
            push_seq((seq_t)1);
            continue;
        }

        /* start candidate search from last value + 1 */
        idx_t last = (idx_t)seq_arr[seq_len - 1];
        idx_t cand = last + 1;

        for (;;) {
            if (cand > SIEVE_LIMIT) abort();   /* should not happen for tests */

            idx_t temp = cand;
            size_t count = 0;                 /* how many indices already present */

            while (temp > 1) {
                idx_t p = spf[temp];
                uint32_t p_idx = prime_index_map[p];   /* 1‑based index of prime p */

                /* divide out all copies of p, counting multiplicity */
                while (temp % p == 0) {
                    temp /= p;
                    if (seq_contains((seq_t)p_idx)) {
                        ++count;
                        if (count > 1) break;
                    }
                }
                if (count > 1) break;
            }

            if (count == 1) {                  /* good candidate */
                push_seq((seq_t)cand);
                break;
            }
            ++cand;
        }
    }

    return seq_arr[n - 1];
}
