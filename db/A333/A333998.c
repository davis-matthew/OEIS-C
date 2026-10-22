/*
 * A333998  Positive even integers 2k such that for all Goldbach partitions
 *          (p,q) of 2k, there exists a prime r_0 in p < r_0 < q.
 * https://oeis.org/A333998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A333998:  Positive even integers 2k such that for all Goldbach partitions (p,q) 
 *            of 2k there exists a prime r0 with p < r0 < q. 
 *            The function searches for the n-th such integer incrementally. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Prime sieve infrastructure ------------------------------------------- */
static unsigned char *isprime       = NULL;   /* true if index is prime */
static idx_t       *next_prime     = NULL;   /* next prime >= i, sentinel at [max+1] */
static uint64_t    *prime_list     = NULL;   /* list of primes */
static size_t      prime_count     = 0;
static size_t      prime_cap       = 0;
static idx_t       max_sieved      = 0;

/* Ensure a full sieve up to 'limit' (inclusive). Rebuilds all arrays. */
static void ensure_prime_sieve(idx_t limit)
{
    if (limit <= max_sieved) return;

    /* Resize and reinitialize prime flag array */
    unsigned char *new_isprime = (unsigned char*)realloc(isprime,
                            (size_t)(limit + 1) * sizeof(unsigned char));
    if (!new_isprime) exit(1);
    isprime = new_isprime;
    for (size_t i = 0; i <= limit; ++i) isprime[i] = 1;
    if (limit >= 0) isprime[0] = 0;
    if (limit >= 1) isprime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) isprime[j] = 0;
        }
    }

    /* Rebuild prime list */
    size_t new_prime_cap = (size_t)limit + 10;
    if (prime_cap < new_prime_cap) {
        uint64_t *np = (uint64_t*)realloc(prime_list, new_prime_cap * sizeof(uint64_t));
        if (!np) exit(1);
        prime_list = np;
        prime_cap  = new_prime_cap;
    }
    size_t new_prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (isprime[i]) prime_list[new_prime_count++] = (uint64_t)i;
    }
    prime_count = new_prime_count;

    /* Rebuild next_prime array */
    idx_t *new_next_prime = (idx_t*)realloc(next_prime,
                            (size_t)(limit + 2) * sizeof(idx_t));
    if (!new_next_prime) exit(1);
    next_prime = new_next_prime;

    next_prime[limit + 1] = limit + 1;   /* sentinel */
    for (int64_t i = (int64_t)limit; i >= 0; --i) {
        next_prime[i] = isprime[i] ? (idx_t)i : next_prime[i + 1];
    }

    max_sieved = limit;
}

/* --- Check whether an even integer is in the sequence -------------------- */
static bool is_valid_even(idx_t even)
{
    if (even == 2) return true;          /* 2 is a special case */

    idx_t half = even >> 1;
    for (size_t idx = 0; idx < prime_count; ++idx) {
        idx_t p = prime_list[idx];
        if (p > half) break;

        idx_t q = even - p;
        if (!isprime[q]) continue;  /* (p,q) is not a Goldbach pair */

        if (p == q) return false;   /* equal primes: interval empty */

        idx_t next = next_prime[p + 1];
        if (next >= q) return false;   /* no prime strictly between */
    }
    return true;
}

/* --- Sequence storage ---------------------------------------------- */
static seq_t *valid_seq = NULL;
static size_t valid_cap = 0;
static size_t valid_cnt = 0;

/* --- Main function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined in OEIS; return 0 as fallback */

    /* Initial sieve limit: enough for the first term */
    static idx_t current_even = 0;
    static idx_t sieve_limit  = 0;
    if (sieve_limit == 0) {
        ensure_prime_sieve(100);        /* start with a moderate range */
        sieve_limit = 100;
    }

    /* Grow valid_seq buffer if needed */
    if (valid_cnt == valid_cap) {
        size_t new_cap = (valid_cap ? valid_cap * 2 : 16);
        seq_t *tmp = (seq_t*)realloc(valid_seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        valid_seq = tmp;
        valid_cap = new_cap;
    }

    /* Find successive terms until we have at least n of them */
    while (valid_cnt < n) {
        current_even += 2;              /* iterate over even numbers */

        if (current_even > sieve_limit) {
            idx_t new_limit = current_even + 1000;
            ensure_prime_sieve(new_limit);
            sieve_limit = new_limit;
        }

        if (is_valid_even(current_even)) {
            valid_seq[valid_cnt++] = (seq_t)current_even;
        }
    }

    return valid_seq[n - 1];
}
