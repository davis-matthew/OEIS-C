/*
 * A375775  For n >= 1, a(n) is the largest k >= 1 such that A001359(n) + i*(i
 *          + 1) is prime for all i from 1 to k.
 * https://oeis.org/A375775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum k such that the lower member of the n‑th twin *
 *            prime p satisfies p + i·(i+1) is prime for all 1 ≤ i ≤ k. * *
 *            The code builds a sieve and collects lower twin primes until it
 *            has * at least n of them, then counts how many consecutive
 *            numbers * p + i·(i+1) are prime. It uses the util...
 * Range:     u8 n <= 3, u16 n <= 861, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/build_primes_upto.h"

static size_t sieve_limit = 0;          /* current sieve limit */
static uint64_t *prime_list = NULL;     /* primes up to sieve_limit */
static size_t prime_count = 0;
static uint64_t *twin_list = NULL;      /* lower members of twin primes */
static size_t twin_count = 0;
static size_t twin_cap = 0;

/* Recompute primes up to 'limit' and rebuild twin list */
static void recompute_twin(size_t limit)
{
    /* free old prime list */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    /* free old twin list */
    if (twin_list) {
        free(twin_list);
        twin_list = NULL;
        twin_cap = 0;
        twin_count = 0;
    }

    /* obtain prime list up to 'limit' */
    uint64_t *tmp_primes = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto((idx_t)limit, &tmp_primes, &tmp_cnt);
    prime_list = tmp_primes;
    prime_count = tmp_cnt;

    /* build twin list */
    twin_cap = 10;               /* initial capacity */
    twin_list = (uint64_t *)malloc(twin_cap * sizeof(uint64_t));
    twin_count = 0;

    for (size_t i = 1; i < prime_count; ++i) {
        if (prime_list[i] == prime_list[i-1] + 2) {
            /* ensure capacity */
            if (twin_count >= twin_cap) {
                twin_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(twin_list, twin_cap * sizeof(uint64_t));
                if (!tmp) break;          /* allocation failure – abort */
                twin_list = tmp;
            }
            twin_list[twin_count++] = prime_list[i-1];
        }
    }
    sieve_limit = limit;
}

/* Ensure that we have at least 'n' lower twin primes */
static void ensure_twin_primes(idx_t n)
{
    if ((size_t)n <= twin_count)
        return;

    size_t limit = sieve_limit ? sieve_limit * 2 : 100;   /* start with 100 if empty */
    while (twin_count < (size_t)n) {
        recompute_twin(limit);
        limit *= 2;   /* enlarge for next round if necessary */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1 */
        return 0;

    ensure_twin_primes(n);

    seq_t p = (seq_t)twin_list[(size_t)n - 1];   /* lower twin prime of nth pair */
    seq_t k = 0;

    for (seq_t i = 1; ; ++i) {
        seq_t val = p + i * (i + 1);          /* p + i(i+1) */
        if (!isPrimeGeneric(val))
            break;
        k = i;
    }
    return k;
}
