/*
 * A323138  Multiples of 6 that are not the sum of two consecutive primes.
 * https://oeis.org/A323138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the OEIS sequence A323138: 
 *     the nth multiple of 6 that is NOT the sum of two consecutive primes.
 * 
 * The implementation incrementally generates primes as needed, marks which
 * multiples of 6 can be expressed as a sum of two consecutive primes, and
 * builds the sequence value array.  It works for any integer type
 * seq_t (including an arbitrary‑precision C++ class) and any idx_t
 * (uint64_t).  It is fully compliant with C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- static state ---------- */
static uint64_t *primes = NULL;
static size_t primes_size = 0;
static size_t primes_cnt = 0;
static uint64_t prev_prime = 0;           /* last prime inserted */

static bool *sum_marked = NULL;           /* true if 6*k can be expressed as a sum of two consecutive primes */
static size_t sum_size = 0;               /* allocated size of sum_marked */

static seq_t *terms = NULL;               /* sequence terms */
static size_t terms_cap = 0;
static size_t terms_cnt = 0;

static uint64_t next_multiple_to_check = 6;  /* next multiple of 6 to examine */

/* ---------- helpers ---------- */
static void ensure_sum_marked(size_t idx)
{
    if (idx < sum_size)
        return;
    size_t new_size = sum_size == 0 ? 64 : sum_size * 2;
    while (new_size <= idx)
        new_size <<= 1;
    bool *new_arr = (bool *)realloc(sum_marked, new_size * sizeof(bool));
    if (!new_arr)
        abort();
    for (size_t i = sum_size; i < new_size; ++i)
        new_arr[i] = false;
    sum_marked = new_arr;
    sum_size = new_size;
}

static void ensure_primes_upto(uint64_t limit)
{
    if (primes_cnt > 0 && primes[primes_cnt - 1] >= limit)
        return;

    /* ensure the primes array has space for new primes */
    if (primes_cnt == primes_size) {
        size_t new_size = primes_size == 0 ? 16 : primes_size * 2;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_size * sizeof(uint64_t));
        if (!tmp)
            abort();
        primes = tmp;
        primes_size = new_size;
    }

    /* seed the sieve with the first prime if empty */
    if (primes_cnt == 0) {
        primes[0] = 2;
        prev_prime = 2;
        primes_cnt = 1;
    }

    uint64_t candidate = primes[primes_cnt - 1] + 1;
    if ((candidate & 1) == 0)
        ++candidate;   /* keep only odd candidates */

    while (candidate <= limit) {
        bool isprime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            /* insert the new prime */
            if (primes_cnt == primes_size) {
                size_t new_size = primes_size * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_size * sizeof(uint64_t));
                if (!tmp)
                    abort();
                primes = tmp;
                primes_size = new_size;
            }
            primes[primes_cnt] = candidate;

            /* mark the sum of this prime with the previous one, if divisible by 6 */
            if (prev_prime != 0) {
                uint64_t sum = prev_prime + candidate;
                if (sum % 6 == 0) {
                    size_t idx = sum / 6;
                    ensure_sum_marked(idx);
                    sum_marked[idx] = true;
                }
            }
            prev_prime = candidate;
            ++primes_cnt;
        }
        candidate += 2;   /* next odd number */
    }
}

static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    terms = tmp;
    terms_cap = new_cap;
}

static void generate_terms_up_to(idx_t n_needed)
{
    while (terms_cnt < (size_t)n_needed) {
        uint64_t m = next_multiple_to_check;
        next_multiple_to_check += 6;

        /* make sure primes up to m are known so we can mark all relevant sums */
        ensure_primes_upto(m);

        size_t idx = m / 6;
        if (idx >= sum_size)
            ensure_sum_marked(idx);

        if (!sum_marked[idx]) {
            ensure_terms_capacity(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)m;
        }
    }
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for 0, but avoid UB */
    generate_terms_up_to(n);
    return terms[n - 1];
}
