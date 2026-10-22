/*
 * A329453  There are exactly two primes in {a(n+i) + a(n+j), 0 <= i < j <= 4}
 *          for any n: lexicographically earliest such sequence of distinct
 *          nonnegative integers.
 * https://oeis.org/A329453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the greedy lexicographically earliest sequence of *
 *            distinct non‑negative integers such that for every five
 *            consecutive * terms, exactly two of the ten pairwise sums are
 *            prime
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
/* primality routine supplied in the build */
#include "../utils/is_prime_big.h"

/* ---------- sequence storage ----------------------------------------------- */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> seqvec;              /* container for C++ */
#else
static seq_t *seq = NULL;                     /* raw array for C */
static size_t seq_len = 0;
static size_t seq_cap = 0;
#endif

/* ensure sufficient capacity for the array */
#ifndef __cplusplus
static void ensure_seq_capacity(idx_t n)
{
    if (n < seq_cap) return;
    size_t new_cap = seq_cap? seq_cap*2 : 16;
    while (new_cap <= (size_t)n) new_cap *= 2;
    seq = (seq_t*) realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}
#endif

/* ---------- used‑number bitset ---------------------------------------------- */
static uint64_t *used_bits = NULL;     /* bit k set if number k already used */
static size_t used_words = 0;          /* words allocated */

static inline void ensure_used_capacity(uint64_t k)
{
    size_t need = (k >> 6) + 1;
    if (need <= used_words) return;
    uint64_t *nbits = (uint64_t*) realloc(used_bits, need * sizeof(uint64_t));
    for (size_t i = used_words; i < need; ++i) nbits[i] = 0;
    used_bits = nbits;
    used_words = need;
}

static inline bool is_used_value(seq_t val)
{
    uint64_t k = (uint64_t)val;
    if (k >> 6 >= used_words) return false;
    return (used_bits[k >> 6] >> (k & 63)) & 1ULL;
}

static inline void mark_used_value(seq_t val)
{
    uint64_t k = (uint64_t)val;
    ensure_used_capacity(k);
    used_bits[k >> 6] |= 1ULL << (k & 63);
}

/* ---------- macros for uniform access -------------------------------------- */
#ifdef __cplusplus
#define SEQ_AT(i)          seqvec[(size_t)(i)]
#define SEQ_PUSH(v)        seqvec.push_back(v)
#define SEQ_LEN            seqvec.size()
#else
#define SEQ_AT(i)          seq[(size_t)(i)]
#define SEQ_PUSH(v)        do{ ensure_seq_capacity(seq_len+1); seq[seq_len++] = v; }while(0)
#define SEQ_LEN            seq_len
#endif

/* ---------- initialisation --------------------------------------------------- */
static void init_sequence(void)
{
    if (SEQ_LEN > 0) return;   /* already initialised */

#ifdef __cplusplus
    seqvec.reserve(5);
    seqvec.push_back((seq_t)0);
    seqvec.push_back((seq_t)1);
    seqvec.push_back((seq_t)2);
    seqvec.push_back((seq_t)8);
    seqvec.push_back((seq_t)14);
#else
    ensure_seq_capacity(4);            /* indices 0..4 */
    seq[0] = (seq_t)0;
    seq[1] = (seq_t)1;
    seq[2] = (seq_t)2;
    seq[3] = (seq_t)8;
    seq[4] = (seq_t)14;
    seq_len = 5;
#endif

    /* mark used values */
    mark_used_value((seq_t)0);
    mark_used_value((seq_t)1);
    mark_used_value((seq_t)2);
    mark_used_value((seq_t)8);
    mark_used_value((seq_t)14);
}

/* ---------- compute the next term ------------------------------------------- */
static seq_t compute_next(void)
{
    /* grab the last four terms (the first four of the new window) */
    seq_t prev[4];
    size_t len = SEQ_LEN;
    for (size_t k = 0; k < 4; ++k) prev[k] = SEQ_AT(len-4 + k);

    /* count primes among the 6 pairwise sums of these four terms */
    int base_prime_count = 0;
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = i+1; j < 4; ++j)
            if (is_prime_big(prev[i] + prev[j])) ++base_prime_count;

    /* required new primes involving the candidate */
    int req_new = 2 - base_prime_count;   /* 0,1, or 2 */

    /* search smallest unused candidate */
    seq_t cand = 0;
    while (true)
    {
        if (is_used_value(cand)) { ++cand; continue; }

        int new_primes = 0;
        for (size_t k = 0; k < 4; ++k)
            if (is_prime_big(prev[k] + cand)) ++new_primes;

        if (new_primes == req_new)
        {
            mark_used_value(cand);
            SEQ_PUSH(cand);
            return cand;
        }
        ++cand;
    }
}

/* ---------- public function -------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sequence();          /* ensure base terms present */

    while (SEQ_LEN <= (size_t)n)
        compute_next();       /* extend until we have nth term */

    return SEQ_AT((size_t)n);
}
