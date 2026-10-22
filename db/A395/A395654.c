/*
 * A395654  Numbers divisible by 6 that are not the sum of a member of A022004
 *          and a member of A022005.
 * https://oeis.org/A395654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive multiple of 6 that cannot be written as
 *            p + q where p is the smallest member of a prime triplet (p, p+2,
 *            p+6) and q is the smallest member of a prime triplet (q, q+4,
 *            q+6)
 * Range:     u8 n <= 18, u16 n <= 4706, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_vec, grow_vec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/grow_seq_vec.h"
#include "../utils/grow_vec.h"

/* ------------------------------------------------------------ */
/*  Global mutable state                                         */
/* ------------------------------------------------------------ */

static bool *prime_sieve = NULL;       /* sieve, prime_sieve[i] == true iff i is prime */
static idx_t prime_limit = 0;          /* current sieve size (last index considered) */

static uint64_t *p_primes = NULL;      /* list of A022004 base primes */
static size_t   p_cnt   = 0;
static size_t   p_cap   = 0;

static uint64_t *q_primes = NULL;      /* list of A022005 base primes */
static size_t   q_cnt   = 0;
static size_t   q_cap   = 0;

static seq_t *terms   = NULL;          /* memoised values of a(1)…a(n) */
static size_t   terms_cnt = 0;
static size_t   terms_cap = 0;

static idx_t last_processed_multiple = 0;   /* last multiple of 6 examined */

/* ------------------------------------------------------------ */
/*  Utility functions                                            */
/* ------------------------------------------------------------ */

static void push_uint64(uint64_t **arr, size_t *cnt, size_t *cap, uint64_t val)
{
    grow_vec(arr, cnt, cap, *cnt + 1);
    (*arr)[(*cnt)++] = val;
}

static void push_seq(seq_t **arr, size_t *cnt, size_t *cap, seq_t val)
{
    grow_seq_vec(arr, cnt, cap, *cnt + 1);
    (*arr)[(*cnt)++] = val;
}

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

/* ------------------------------------------------------------ */
/*  Prime sieve and list construction                          */
/* ------------------------------------------------------------ */

static void rebuild_prime_lists(void)
{
    /* free old arrays if any */
    free(p_primes); free(q_primes);
    p_cnt = q_cnt = 0;
    p_cap = q_cap = 0;
    p_primes = NULL; q_primes = NULL;

    /* approximate size of prime list */
    size_t est = (prime_limit > 2) ? (prime_limit / 10 + 10) : 16;

    p_primes = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!p_primes) abort();
    p_cap = est;

    q_primes = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!q_primes) abort();
    q_cap = est;

    for (idx_t p = 2; p <= prime_limit; ++p)
    {
        if (!prime_sieve[p]) continue;

        /* A022004: p, p+2, p+6 all prime */
        if (p + 6 <= prime_limit &&
            prime_sieve[p + 2] && prime_sieve[p + 6])
            push_uint64(&p_primes, &p_cnt, &p_cap, (uint64_t)p);

        /* A022005: p, p+4, p+6 all prime */
        if (p + 6 <= prime_limit &&
            prime_sieve[p + 4] && prime_sieve[p + 6])
            push_uint64(&q_primes, &q_cnt, &q_cap, (uint64_t)p);
    }

    /* sort q_primes for binary search */
    qsort(q_primes, q_cnt, sizeof(uint64_t), cmp_uint64);
}

/* expand sieve to at least LIMIT (inclusive) */
static void expand_sieve(idx_t LIMIT)
{
    if (LIMIT <= prime_limit) return;

    bool *new_sieve = (bool *)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (!new_sieve) abort();

    /* initially mark all as prime */
    for (size_t i = 0; i <= (size_t)LIMIT; ++i) new_sieve[i] = true;
    if (LIMIT >= 0) new_sieve[0] = false;
    if (LIMIT >= 1) new_sieve[1] = false;

    for (idx_t i = 2; (idx_t)(i * i) <= LIMIT; ++i)
    {
        if (!new_sieve[i]) continue;
        for (idx_t j = (idx_t)(i * i); j <= LIMIT; j += i)
            new_sieve[j] = false;
    }

    free(prime_sieve);
    prime_sieve = new_sieve;
    prime_limit = LIMIT;

    rebuild_prime_lists();
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                      */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe */

    /* initialise sieve and prime lists once */
    if (prime_limit == 0) expand_sieve(200);

    /* generate terms until we have reached index n */
    while (terms_cnt < (size_t)n)
    {
        /* next multiple of 6 to test */
        idx_t k = last_processed_multiple + 6;

        /* ensure sieve covers k */
        if (k > prime_limit)
        {
            idx_t lim = prime_limit ? prime_limit : 2;
            while (lim < k) lim <<= 1;
            expand_sieve(lim);
        }

        /* test whether k can be expressed as p + q */
        bool representable = false;
        for (size_t i = 0; i < p_cnt && p_primes[i] <= (uint64_t)k; ++i)
        {
            idx_t p = (idx_t)p_primes[i];
            idx_t q = k - p;
            uint64_t q_val = (uint64_t)q;
            if (bsearch(&q_val, q_primes, q_cnt, sizeof(uint64_t), cmp_uint64))
            {
                representable = true;
                break;
            }
        }

        if (!representable)
            push_seq(&terms, &terms_cnt, &terms_cap, (seq_t)k);

        last_processed_multiple = k;
    }

    return terms[(size_t)(n - 1)];
}
