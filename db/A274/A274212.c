/*
 * A274212  The factorization of n contains only lesser of twin primes.
 * https://oeis.org/A274212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9673, u64 n <= 9673,
 *            u128 n <= 9673, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A274212
   Definition: a(n) is the nth positive integer whose prime factors are all
   the smaller prime of a twin prime pair (i.e. 3, 5, 11, 17, 29, …).
   The algorithm generates the sequence as the list of all numbers that
   factor entirely into this set of primes, sorted increasingly. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static state for memoisation */
static seq_t *seq = NULL;          /* array of generated terms */
static size_t seq_len = 0;         /* number of terms stored */
static size_t seq_cap = 0;         /* capacity of seq array */

static uint64_t *twin_primes = NULL; /* list of lesser twin primes */
static size_t twin_cnt = 0;          /* number of twin primes known */

static size_t *idx = NULL;        /* current index per prime */
static seq_t *cand = NULL;        /* current candidate per prime */

/* we initialise everything on the first call */
static void init(void)
{
    if (seq != NULL)
        return; /* already initialised */

    /* initialise the first term a(1)=1 */
    seq_cap = 1024;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    if (!seq) abort();
    seq[0] = (seq_t)1;
    seq_len = 1;

    /* generate twin primes up to a fixed limit sufficient for the first
       few thousand terms.  2,000,000 is ample for n ≤ 10 000. */
    const idx_t LIMIT = 2000000;
    size_t lim = (size_t)LIMIT;

    /* sieve of Eratosthenes to mark primes */
    uint8_t *is_prime = (uint8_t*)calloc(lim + 1, 1);
    if (!is_prime) abort();
    for (size_t i = 2; i <= lim; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= lim; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= lim; j += i)
                is_prime[j] = 0;

    /* count twin primes */
    for (size_t i = 3; i + 2 <= lim; i += 2)
        if (is_prime[i] && is_prime[i + 2])
            ++twin_cnt;

    twin_primes = (uint64_t*)malloc(twin_cnt * sizeof(uint64_t));
    if (!twin_primes) abort();
    size_t k = 0;
    for (size_t i = 3; i + 2 <= lim; i += 2)
        if (is_prime[i] && is_prime[i + 2])
            twin_primes[k++] = (uint64_t)i;

    free(is_prime);

    /* initialise indices and candidate values for each prime */
    idx = (size_t*)malloc(twin_cnt * sizeof(size_t));
    cand = (seq_t*)malloc(twin_cnt * sizeof(seq_t));
    if (!idx || !cand) abort();
    for (size_t i = 0; i < twin_cnt; ++i) {
        idx[i] = 0;
        cand[i] = (seq_t)twin_primes[i];  /* seq[0] == 1 */
    }
}

seq_t a(idx_t n)
{
    init();

    /* satisfy requests with non‑increasing n by generating more terms
       until we have at least n of them. */
    while (seq_len < (size_t)n) {
        /* find the smallest candidate */
        seq_t best = cand[0];
        size_t best_i = 0;
        for (size_t i = 1; i < twin_cnt; ++i)
            if (cand[i] < best) {
                best = cand[i];
                best_i = i;
            }

        /* append the new term */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap * 2 + 1;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            if (!seq) abort();
        }
        seq[seq_len++] = best;

        /* update all primes that produced this value */
        for (size_t i = 0; i < twin_cnt; ++i)
            if (cand[i] == best) {
                ++idx[i];
                cand[i] = (seq_t)twin_primes[i] * seq[idx[i]];
            }
    }

    return seq[(size_t)n - 1];
}
