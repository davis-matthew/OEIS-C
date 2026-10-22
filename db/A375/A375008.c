/*
 * A375008  Products m of k = 3 consecutive primes p_1..p_k, where only p_1 <
 *          m^(1/k).
 * https://oeis.org/A375008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 130, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_prime_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A375008: products of 3 consecutive primes p1,p2,p3 such that
   p1 < (p1 p2 p3)^(1/3) < p2  (equivalently p1³ < p1 p2 p3 < p2³). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_prime_arr.h"

/* Grow a dynamic array of seq_t (used for the output sequence). */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap;
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Simple deterministic primality test using the primes stored so far. */
static bool is_prime_candidate(uint64_t n, const uint64_t *primes, size_t primecnt)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primecnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Find the next prime after `curr` and store it in the prime array. */
static void ensure_prime(uint64_t *curr, uint64_t **primes,
                         size_t *primecnt, size_t *primecap)
{
    while (true) {
        if (is_prime_candidate(*curr, *primes, *primecnt)) {
            grow_prime_arr(primes, primecap, *primecnt + 1);
            (*primes)[(*primecnt)++] = *curr;
            (*curr)++;          /* next candidate will start from the next number */
            break;
        }
        (*curr)++;
    }
}

/* Return the n‑th term of A375008.  n is 1‑based. */
seq_t a(idx_t n)
{
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;
    static uint64_t next_prime_candidate = 2;

    static seq_t *output = NULL;
    static size_t out_cnt = 0, out_cap = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Ensure we have at least the first three primes. */
        while (prime_cnt < 3)
            ensure_prime(&next_prime_candidate, &prime_list, &prime_cnt, &prime_cap);
        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (out_cnt < n) {
        /* Make sure the last triple is up to date. */
        while (prime_cnt < 3)
            ensure_prime(&next_prime_candidate, &prime_list, &prime_cnt, &prime_cap);

        uint64_t p1 = prime_list[prime_cnt - 3];
        uint64_t p2 = prime_list[prime_cnt - 2];
        uint64_t p3 = prime_list[prime_cnt - 1];

        seq_t prod  = (seq_t)p1 * p2 * p3;
        seq_t p1cub = (seq_t)p1 * p1 * p1;
        seq_t p2cub = (seq_t)p2 * p2 * p2;

        if (prod > p1cub && prod < p2cub) {
            grow_seq_arr(&output, &out_cap, out_cnt + 1);
            output[out_cnt++] = prod;
        }

        /* Prepare for next prime. */
        if (next_prime_candidate <= p3) next_prime_candidate = p3 + 1;
        ensure_prime(&next_prime_candidate, &prime_list, &prime_cnt, &prime_cap);
    }

    return output[n - 1];
}
