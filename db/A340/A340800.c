/*
 * A340800  a(n) is the least prime p = prime(k) such that the next prime with
 *          the same last digit as p is prime(k+n).
 * https://oeis.org/A340800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A340800.
   a(n) is the least prime p = prime(k) such that the next prime with the
   same last decimal digit as p is prime(k+n).  The algorithm streams
   primes in ascending order, keeping for each last digit the index of
   its most recent occurrence.  When a new prime with the same digit
   appears, the distance to the previous one determines a candidate
   value for that distance.  The first such candidate for each distance
   is stored; later candidates are ignored.  The routine is independent
   of call order and expands its cache lazily.  It uses the
   build_primes_upto helper from the utils library. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static seq_t *answers = NULL;          /* answers[1..] holds a(n) */
static size_t answers_cap = 0;          /* current allocated size (1‑based) */

static uint64_t *prime_list = NULL;    /* array of primes (uint64_t) */
static size_t prime_cnt = 0;           /* number of primes stored   */
static uint64_t prime_limit = 0;       /* numeric limit of stored primes */
static size_t prime_processed = 0;     /* number of primes already processed */

static size_t last_digit_index[10];    /* 1‑based index of last prime per last digit */

static void init_state(void)
{
    static bool initialized = false;
    if (initialized) return;
    answers = NULL;
    answers_cap = 0;
    prime_list = NULL;
    prime_cnt = 0;
    prime_limit = 0;
    prime_processed = 0;
    for (int i = 0; i < 10; ++i) last_digit_index[i] = 0;
    initialized = true;
}

static void extend_answers(size_t new_cap)
{
    if (new_cap <= answers_cap) return;
    seq_t *tmp = (seq_t *)realloc(answers, sizeof(seq_t) * (new_cap + 1));
    if (!tmp) abort();
    /* zero the new part */
    for (size_t i = answers_cap + 1; i <= new_cap; ++i) tmp[i] = (seq_t)0;
    answers = tmp;
    answers_cap = new_cap;
}

static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit && prime_cnt > prime_processed) return;
    if (prime_list) free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for offset 0 */
    init_state();

    size_t N = (size_t)n;
    if (N > answers_cap) extend_answers(N);

    /* Iterate until we have found a(n). */
    while (answers[N] == (seq_t)0)
    {
        /* Need more primes? */
        if (prime_processed >= prime_cnt)
        {
            uint64_t new_limit = prime_limit * 2 + 1000;
            if (new_limit < 1000000) new_limit = 1000000;
            ensure_primes(new_limit);
        }

        uint64_t p = prime_list[prime_processed];  /* current prime value */
        size_t k = prime_processed + 1;            /* 1‑based index of current prime */
        int d = (int)(p % 10);

        if (last_digit_index[d] != 0)
        {
            size_t j = last_digit_index[d];        /* previous prime index with same last digit */
            size_t dist = k - j;                   /* distance */
            if (dist <= answers_cap)
            {
                if (answers[dist] == (seq_t)0)
                    answers[dist] = (seq_t)prime_list[j - 1];
            }
            else
            {
                extend_answers(dist);
                if (answers[dist] == (seq_t)0)
                    answers[dist] = (seq_t)prime_list[j - 1];
            }
        }

        last_digit_index[d] = k;
        ++prime_processed;
    }

    return answers[N];
}
