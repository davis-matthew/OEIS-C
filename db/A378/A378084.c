/*
 * A378084  Nonsquarefree numbers not appearing in A377783 (least
 *          nonsquarefree number > prime(n)).
 * https://oeis.org/A378084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A378084
   Non‑squarefree numbers that are NOT the least non‑squarefree number
   greater than some prime.  The sequence is computed by:
   – constructing a sieve of primes and identifying all non‑squarefree
     numbers up to a growing limit;
   – for every prime p, marking the smallest non‑squarefree number > p
     as excluded;
   – collecting all non‑squarefree numbers that are not excluded.
   This yields the terms a(1), a(2), … in any order of calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------------*/
/*  Global arrays shared by all calls                               */
/*------------------------------------------------------------------*/
static idx_t limit = 0;              /* current limit of generated tables */
static bool   *is_prime          = NULL;   /* sieve of primes          */
static bool   *is_nonsquarefree = NULL;   /* true ⇔ number is not squarefree */
static uint32_t *prev_prime     = NULL;   /* largest prime ≤ i         */
static uint32_t *next_non_sf    = NULL;   /* smallest non‑squarefree > i */
static bool   *excluded         = NULL;   /* true if number is excluded */
static seq_t  *seq              = NULL;   /* sequence values */
static size_t  seq_len  = 0;              /* number of collected terms   */
static size_t  seq_cap  = 0;              /* current capacity of seq     */
static idx_t   check_ptr = 2;             /* next integer to test       */

/*------------------------------------------------------------------*/
/*  Recompute all tables up to new_limit (inclusive).  All tables
   are rebuilt from scratch.  Memory is re‑allocated with explicit
   casts to avoid C++ compilation errors.  ------------------------------------------------------------------*/
static void recompute(idx_t new_limit)
{
    size_t sz = (size_t)new_limit + 1;
    is_prime          = (bool*)realloc(is_prime,          sz * sizeof(bool));
    is_nonsquarefree  = (bool*)realloc(is_nonsquarefree,  sz * sizeof(bool));
    prev_prime        = (uint32_t*)realloc(prev_prime,        sz * sizeof(uint32_t));
    next_non_sf      = (uint32_t*)realloc(next_non_sf,      sz * sizeof(uint32_t));
    excluded          = (bool*)realloc(excluded,          sz * sizeof(bool));
    if (!is_prime || !is_nonsquarefree || !prev_prime ||
        !next_non_sf || !excluded)
        exit(1);   /* out of memory */

    /* initialise arrays */
    for (idx_t i = 0; i <= new_limit; ++i) {
        is_prime[i]          = true;
        is_nonsquarefree[i]  = false;
        prev_prime[i]        = 0;
        next_non_sf[i]       = 0;
        excluded[i]          = false;
    }
    if (new_limit >= 0) is_prime[0] = false;
    if (new_limit >= 1) is_prime[1] = false;

    /* Sieve of Eratosthenes for primes */
    for (idx_t i = 2; (unsigned long long)i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = false;
        }
    }

    /* prev_prime array */
    uint32_t last = 0;
    for (idx_t i = 0; i <= new_limit; ++i) {
        if (is_prime[i]) last = (uint32_t)i;
        prev_prime[i] = last;
    }

    /* Mark non‑squarefree numbers */
    for (idx_t i = 2; (unsigned long long)i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            unsigned long long sq = (unsigned long long)i * (unsigned long long)i;
            for (unsigned long long j = sq; j <= new_limit; j += sq)
                is_nonsquarefree[(size_t)j] = true;
        }
    }

    /* next_non_sf: smallest non‑squarefree > i   (0 if none) */
    uint32_t last_nf = 0;
    for (idx_t i = new_limit; i > 0; --i) {
        next_non_sf[(size_t)i] = last_nf;
        if (is_nonsquarefree[(size_t)i]) last_nf = (uint32_t)i;
        if (i == 1) break;   /* prevent unsigned underflow */
    }

    /* Mark excluded numbers: for each prime p, the next
       non‑squarefree number after p is excluded. */
    for (idx_t p = 2; p <= new_limit; ++p) {
        if (is_prime[p]) {
            uint32_t nf = next_non_sf[(size_t)p];
            if (nf != 0)
                excluded[(size_t)nf] = true;
        }
    }

    limit = new_limit;
}

/*------------------------------------------------------------------*/
/*  Main entry point: returns a(n) (1‑based).  ---------------------*/
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* should not happen for valid n ≥ 1 */

    /* Grow limit until we have enough pre‑computed tables */
    while (seq_len < n) {
        /* Ensure tables cover the current check_ptr */
        if (check_ptr > limit) {
            idx_t new_limit = (limit == 0 ? 1000 : limit << 1);
            if (new_limit < check_ptr) new_limit = check_ptr << 1;
            recompute(new_limit);
            /* After recompute, the tables are fresh; check_ptr stays unchanged */
        }

        /* Scan numbers in the current table range collecting new terms */
        while (check_ptr <= limit && seq_len < n) {
            if (is_nonsquarefree[(size_t)check_ptr] && !excluded[(size_t)check_ptr]) {
                /* store the number */
                if (seq_len == seq_cap) {
                    size_t new_cap = (seq_cap == 0 ? 32 : seq_cap << 1);
                    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                    if (!seq) exit(1);   /* out of memory */
                    seq_cap = new_cap;
                }
                seq[seq_len++] = (seq_t)check_ptr;
            }
            ++check_ptr;
        }
    }

    return seq[n - 1];
}
