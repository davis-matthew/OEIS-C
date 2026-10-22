/*
 * A378917  a(n) is the smallest positive integer having n decompositions of
 *          twice its value into ordered sums of two odd primes.
 * https://oeis.org/A378917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest positive integer m such that the number of
 *            ordered decompositions of 2*m into sums of two odd primes equals
 *            n. The function performs an incremental search of m, builds a
 *            prime table by sieve, counts the decompositions, and stores the
 *            first m for each count encountered
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- prime table ---------- */
static bool *prime_flags = NULL;
static idx_t prime_flags_limit = 0;
static idx_t *primes = NULL;
static size_t primes_cnt = 0;

/* ensure prime_flags[0..limit] exists, and primes up to limit are built */
static void ensure_limit(idx_t limit)
{
    if (limit <= prime_flags_limit)
        return;

    /* extend the flag array */
    bool *new_flags = (bool*)realloc(prime_flags, (limit + 1) * sizeof(bool));
    if (!new_flags) return;          /* unlikely */
    /* initialise new entries to true */
    for (idx_t i = prime_flags_limit + 1; i <= limit; ++i)
        new_flags[i] = true;
    new_flags[0] = false;
    if (limit >= 1)
        new_flags[1] = false;
    prime_flags = new_flags;

    /* sieve new range */
    for (idx_t i = 2; i * (idx_t)i <= limit; ++i) {
        if (prime_flags[i]) {
            idx_t start = i * i;
            for (idx_t j = start; j <= limit; j += i)
                prime_flags[j] = false;
        }
    }
    prime_flags_limit = limit;

    /* rebuild primes list (odd primes ≥3) */
    size_t new_cnt = 0;
    for (idx_t i = 3; i <= limit; i += 2)
        if (prime_flags[i])
            ++new_cnt;

    idx_t *new_primes = (idx_t*)realloc(primes, new_cnt * sizeof(idx_t));
    if (!new_primes) return;        /* unlikely */
    idx_t pos = 0;
    for (idx_t i = 3; i <= limit; i += 2)
        if (prime_flags[i])
            new_primes[pos++] = i;
    primes = new_primes;
    primes_cnt = new_cnt;
}

/* ---------- answer cache ---------- */
static seq_t *answer_cache = NULL;   /* answer_cache[count] = first m */
static size_t answer_cache_size = 0;

/* current maximum m processed */
static idx_t current_m = 0;

seq_t a(idx_t n)
{
    /* initialise cache for n = 0 (a(0)=1) */
    if (!answer_cache) {
        answer_cache_size = 1;
        answer_cache = (seq_t*)calloc(answer_cache_size, sizeof(seq_t));
        answer_cache[0] = (seq_t)1;
    }

    /* if answer already known, finish */
    if (n < answer_cache_size && answer_cache[n] != 0)
        return answer_cache[n];

    /* ensure prime table ready (start with small limit) */
    if (!prime_flags)
        ensure_limit(2);

    /* incremental search for m */
    while (n >= answer_cache_size || answer_cache[n] == 0) {
        ++current_m;
        idx_t m = current_m;
        idx_t N = 2 * m;
        ensure_limit(N);

        /* count ordered decompositions of N into odd primes */
        int count = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t p = primes[i];
            if (p > N / 2) break;
            idx_t q = N - p;
            if (q >= 3 && prime_flags[q])
                count += (p == q) ? 1 : 2;
        }

        /* grow cache if needed */
        if ((size_t)count >= answer_cache_size) {
            size_t new_size = answer_cache_size > 0 ? answer_cache_size << 1 : 1;
            while (new_size <= (size_t)count)
                new_size <<= 1;
            seq_t *tmp = (seq_t*)realloc(answer_cache, new_size * sizeof(seq_t));
            if (!tmp) return 0;            /* unlikely failure */
            for (size_t i = answer_cache_size; i < new_size; ++i)
                tmp[i] = 0;
            answer_cache = tmp;
            answer_cache_size = new_size;
        }

        /* store first m for this count if not already stored */
        if (answer_cache[count] == 0)
            answer_cache[count] = (seq_t)m;
    }

    return answer_cache[n];
}
