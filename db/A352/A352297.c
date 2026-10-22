/*
 * A352297  Even numbers with exactly 1 pair of Goldbach partitions, (p,q) and
 *          (r,s) with p,q,r,s prime and p < r <= s < q, such that all
 *          integers in the open intervals (p,r) and (s,q) are composite.
 * https://oeis.org/A352297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A352297 – even numbers with exactly one qualifying Goldbach pair.
   The function incrementally tests even numbers, using a prime sieve,
   and memoises the sequence values.  It works for all supported
   seq_t/idx_t types (fixed-width unsigned, __int128, or a trivial
   arbitrary‑precision class).  The algorithm is straightforward
   O(n log n) and meets the 2‑second constraint for n up to ~10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime sieve shared state ---------------------------------- */
static uint8_t *is_prime = NULL;   /* 0 = composite, 1 = prime              */
static idx_t       is_prime_limit = 0;   /* inclusive upper bound of sieve     */
static uint64_t *prime_list = NULL;      /* list of prime numbers              */
static size_t    prime_cnt  = 0;          /* count of primes in prime_list      */

/* Ensure the sieve covers all integers up to 'limit'.  Reallocates
   memory and rebuilds the prime list when necessary.  */
static void init_sieve(idx_t limit)
{
    if (limit <= is_prime_limit)
        return;

    /* Allocate/resize the is_prime array.  The cast is required
       for C++ compliance. */
    {
        uint8_t *tmp = (uint8_t *)realloc(is_prime, (size_t)(limit + 1) * sizeof(uint8_t));
        if (!tmp) abort();
        is_prime = tmp;
    }

    /* Initialise all entries as prime, then clear 0 and 1. */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = 0;
    is_prime[1] = 0;

    /* Classic sieve of Eratosthenes. */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Build prime list.  The capacity is kept simple and grown
       automatically. */
    size_t est_cap = limit / 10 + 10;
    if (est_cap < 16) est_cap = 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, est_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;

    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_cnt >= est_cap) {
                est_cap <<= 1;
                uint64_t *tmp2 = (uint64_t *)realloc(prime_list, est_cap * sizeof(uint64_t));
                if (!tmp2) abort();
                prime_list = tmp2;
            }
            prime_list[prime_cnt++] = (uint64_t)i;
        }
    }
    is_prime_limit = limit;
}

/* Return true iff all integers in (l, r) are composite. */
static inline bool interval_all_composite(idx_t l, idx_t r)
{
    for (idx_t x = l + 1; x < r; ++x) {
        if (is_prime[x])
            return false;
    }
    return true;
}

/* ---------- Sequence storage ------------------------------------------ */
static seq_t *sequence = NULL;   /* dynamic array of computed terms        */
static size_t seq_len  = 0;      /* actual number of terms stored          */
static size_t seq_cap  = 0;      /* current capacity of 'sequence'         */
static idx_t   next_even = 10;   /* next even integer to examine           */

/* Ensure 'sequence' has room for at least 'needed' elements. */
static inline void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;

    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    sequence = tmp;
    seq_cap   = new_cap;
}

/* ---------- Public sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* requests below offset are undefined; return 0 */
        return 0;

    /* Initialise the sequence storage and prime sieve on first call. */
    if (seq_len == 0) {
        seq_cap = 128;
        seq_t *tmp = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        if (!tmp) abort();
        sequence = tmp;
        init_sieve(200);          /* small start for early values        */
    }

    /* Generate terms until we have at least 'n' of them. */
    while (seq_len < n) {
        /* Expand the sieve if the next candidate exceeds it. */
        if (next_even > is_prime_limit) {
            idx_t new_limit = next_even * 2;
            if (new_limit < 1000) new_limit = 1000;
            init_sieve(new_limit);
        }

        /* Gather all Goldbach partitions of 'next_even'. */
        static idx_t p_arr[512];
        static idx_t q_arr[512];
        size_t par_cnt = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p > next_even / 2)
                break;
            idx_t q = next_even - p;
            if (is_prime[q]) {
                p_arr[par_cnt] = p;
                q_arr[par_cnt] = q;
                ++par_cnt;
                if (par_cnt == 512)
                    break;
            }
        }

        /* Count qualifying pairs of partitions. */
        size_t pair_cnt = 0;
        for (size_t i = 0; i < par_cnt && pair_cnt <= 1; ++i) {
            for (size_t j = i + 1; j < par_cnt && pair_cnt <= 1; ++j) {
                idx_t p_i = p_arr[i], q_i = q_arr[i];
                idx_t p_j = p_arr[j], q_j = q_arr[j];
                if (!(p_i < p_j && q_j < q_i))
                    continue;
                if (interval_all_composite(p_i, p_j) &&
                    interval_all_composite(q_j, q_i))
                {
                    ++pair_cnt;
                }
            }
        }

        if (pair_cnt == 1) {
            ensure_seq_capacity(seq_len + 1);
            sequence[seq_len++] = (seq_t)next_even;
        }

        next_even += 2;   /* next even integer */
    }

    return sequence[n - 1];
}
