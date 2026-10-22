/*
 * A322676  Numbers k > 0 such that k has more divisors d, such that d-1 is
 *          prime, than any other smaller positive number, with a(1) = 1.
 * https://oeis.org/A322676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this function returns the nth record k > 0 such that k has more
 *            divisors d with d-1 prime than any smaller positive number. The
 *            computation is performed lazily: the sequence is built up to the
 *            requested index n and cached for future calls. The algorithm
 *            counts, for each integer k, how many prime‑...
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"       /* seq_alloc, seq_realloc */
#include "../utils/seq_realloc.h"     /* seq_realloc */

static idx_t prime_sieve_limit = 0;          /* largest n for which primes
                                                 have been computed */
static uint32_t *primes = NULL;              /* array of primes */
static size_t prime_cnt = 0;

static int *cnt = NULL;                      /* f(k) values for k <=
                                                 current_k_allocated */
static idx_t current_k_allocated = 0;         /* highest k for which f is computed */

static size_t rec_cap = 0;                   /* capacity of records array */
static size_t rec_cnt = 0;                   /* number of records stored */
static seq_t *records = NULL;                /* list of record k values
                                                 (1‑based) */

static int max_f_so_far = 0;                 /* current record maximum f */

static bool initialized = false;

/* -------------------------------------------------------------------------- */
/*  Sieve of Eratosthenes: compute all primes up to limit (inclusive). */
/*  Overwrites the global primes array and prime_cnt.                        */
/* -------------------------------------------------------------------------- */
static void ensure_prime_sieve(idx_t limit)
{
    if (prime_sieve_limit >= limit) return;

    size_t n = (size_t)limit;
    unsigned char *is_prime = (unsigned char *)calloc(n + 1, sizeof(unsigned char));
    if (!is_prime) abort();

    for (size_t i = 2; i <= n; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= n; j += i) is_prime[j] = 0;
        }
    }

    size_t estimate = n / 10 + 10;
    uint32_t *tmp = primes;
    if (!tmp) tmp = (uint32_t *)malloc(estimate * sizeof(uint32_t));
    else     tmp = (uint32_t *)realloc(tmp, estimate * sizeof(uint32_t));
    if (!tmp) abort();
    primes = tmp;

    prime_cnt = 0;
    for (size_t i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == estimate) {
                estimate *= 2;
                uint32_t *t2 = (uint32_t *)realloc(primes, estimate * sizeof(uint32_t));
                if (!t2) abort();
                primes = t2;
            }
            primes[prime_cnt++] = (uint32_t)i;
        }
    }

    free(is_prime);
    prime_sieve_limit = limit;
}

/* -------------------------------------------------------------------------- */
/*  Extend precomputation up to target (inclusive).                          */
/*  Updates cnt array, primes, and records list.                             */
/* -------------------------------------------------------------------------- */
static void extend_to(idx_t target)
{
    if (target <= current_k_allocated) return;

    /* Ensure primes are available up to target-1. */
    if (target > 0) ensure_prime_sieve(target - 1);

    /* Resize cnt array */
    int *new_cnt = (int *)realloc(cnt, (size_t)(target + 1) * sizeof(int));
    if (!new_cnt) abort();
    /* Initialize newly allocated entries */
    for (idx_t i = current_k_allocated + 1; i <= target; ++i) new_cnt[(size_t)i] = 0;
    cnt = new_cnt;

    /* Increment counts for all prime+1 divisors */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = primes[pi];
        idx_t sp = (idx_t)p + 1;      /* divisor value */
        if (sp > target) break;

        idx_t start;
        if (sp > current_k_allocated) {
            start = sp;
        } else {
            start = ((current_k_allocated / sp) + 1) * sp;
        }

        for (idx_t m = start; m <= target; m += sp) {
            cnt[(size_t)m]++;          /* m divisible by sp */
        }
    }

    /* Scan new k for new record values */
    for (idx_t k = current_k_allocated + 1; k <= target; ++k) {
        int fk = cnt[(size_t)k];
        if (fk > max_f_so_far) {
            max_f_so_far = fk;
            /* Grow records array if needed */
            if (rec_cnt == rec_cap) {
                size_t new_cap = rec_cap == 0 ? 8 : rec_cap * 2;
                seq_t *tmp = seq_realloc(records, rec_cap, new_cap);
                if (!tmp) abort();
                records = tmp;
                rec_cap = new_cap;
            }
            records[rec_cnt++] = (seq_t)k;
        }
    }

    current_k_allocated = target;
}

/* -------------------------------------------------------------------------- */
/*  Public sequence function                                               */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;

        /* Allocate minimal buffers */
        cnt = (int *)calloc(2, sizeof(int));          /* indices 0 and 1 */
        if (!cnt) abort();
        current_k_allocated = 1;                      /* f(1) already known (0) */

        max_f_so_far = cnt[1];                        /* 0 */

        rec_cap = 8;
        records = seq_alloc(rec_cap);                  /* seq_alloc: zero‑init */
        if (!records) abort();
        records[0] = (seq_t)1;                        /* a(1) = 1 */
        rec_cnt = 1;
    }

    /* Keep extending until we have at least n records */
    while (rec_cnt < n) {
        idx_t next_target = current_k_allocated * 2;
        if (next_target < 2) next_target = 2;
        extend_to(next_target);
    }

    return records[n - 1];
}
