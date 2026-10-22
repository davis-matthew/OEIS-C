/*
 * A357981  Numbers whose prime indices have only prime numbers as their own
 *          prime indices.
 * https://oeis.org/A357981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer whose prime indices have only prime
 *            indices as their own prime indices
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Pre‑computed tables (initialized lazily) */
static uint32_t *spf = NULL;          /* smallest prime factor for each value */
static uint32_t *prime_idx = NULL;    /* 1‑based index of a prime (0 for composites) */
static bool *is_good_index = NULL;    /* true if an index satisfies the condition */
static bool *valid_prime = NULL;      /* true if a prime is allowed as a factor */

static seq_t *seq = NULL;             /* cache of sequence values */
static size_t seq_len = 0, seq_cap = 0;
static idx_t next_candidate = 1;

/* limit big enough for >10 000 terms (empirical) */
#define LIMIT 2000000

/* ------------------------------------------------------------------------- */
/* Helper: build linear sieve up to LIMIT, populate spf, prime_idx, arrays */
static void init_tables(void)
{
    if (spf != NULL)                /* already done */
        return;

    /* linear sieve for smallest prime factors and prime list           */
    spf = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    prime_idx = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));

    /* allocate space for primes (rough upper bound for ~LIMIT primes) */
    uint32_t *pr = (uint32_t*)malloc((LIMIT / 10) * sizeof(uint32_t));
    size_t pr_cnt = 0;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                  /* i is prime */
            spf[i] = i;
            pr[pr_cnt++] = i;
        }
        for (size_t j = 0; j < pr_cnt && pr[j] <= spf[i] &&
                (uint64_t)i * pr[j] <= LIMIT; ++j)
            spf[i * pr[j]] = pr[j];
    }

    /* prime_idx[prime] = its 1‑based index in the prime list           */
    for (size_t i = 0; i < pr_cnt; ++i)
        prime_idx[pr[i]] = (uint32_t)(i + 1);

    /* compute good indices: all prime factors of an index must have
       prime indices themselves                                 */
    size_t max_index = pr_cnt;   /* maximum possible index */
    is_good_index = (bool*)calloc(max_index + 1, sizeof(bool));
    is_good_index[1] = true;     /* empty product */

    for (size_t i = 2; i <= max_index; ++i) {
        bool good = true;
        uint32_t temp = (uint32_t)i;
        while (temp > 1) {
            uint32_t q = spf[temp];                /* prime factor of i            */
            uint32_t idx_q = prime_idx[q];          /* index of that prime        */
            if (idx_q == 0 || spf[idx_q] != idx_q) { /* idx_q not prime */
                good = false;
                break;
            }
            while (temp % q == 0)
                temp /= q;
        }
        is_good_index[i] = good;
    }

    /* which primes are admissible as factors?                */
    valid_prime = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (size_t i = 0; i < pr_cnt; ++i) {
        uint32_t p = pr[i];
        uint32_t m = prime_idx[p];
        if (m <= max_index && is_good_index[m])
            valid_prime[p] = true;
    }

    free(pr);    /* primes array no longer needed */
}

/* ------------------------------------------------------------------------- */
/* extend sequence cache to at least 'need' elements */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------------- */
/* test whether an integer satisfies the A357981 criterion              */
static bool is_valid_candidate(idx_t n)
{
    if (n == 1)               /* 1 is always allowed                           */
        return true;
    idx_t x = n;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];          /* smallest prime factor            */
        if (!valid_prime[p])
            return false;
        while (x % p == 0)
            x /= p;
    }
    return true;
}

/* ------------------------------------------------------------------------- */
/* core function ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_tables();                /* ensure tables are ready                   */

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        if (is_valid_candidate(next_candidate)) {
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq[n - 1];
}
