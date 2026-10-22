/*
 * A375712  Numbers k such that A013929(k+1) - A013929(k) = 4. In other words,
 *          the k-th nonsquarefree number is 4 less than the next
 *          nonsquarefree number.
 * https://oeis.org/A375712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that the k‑th nonsquarefree number is 4 less than
 *            the next nonsquarefree number. In other words, a(n) records the
 *            indices k where A013929(k+1) – A013929(k) = 4
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation keeps a dynamic sieve that marks all composite
    numbers containing a square factor, and generates nonsquarefree
    numbers in ascending order.  When a gap of four between two
    successive nonsquarefree numbers is detected, the index of the
    first of the two is stored in a table ks.
    The function is thread‑safe in the sense that subsequent calls can
    be made in any order; the table is extended lazily until the
    n‑th term is available. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/*  global state                                                      */
/* --------------------------------------------------------------------- */

/* Sieve table: 1 = nonsquarefree, 0 = squarefree (or <4). */
static uint8_t *sieve      = NULL;          /* allocated up to sieve_cap-1 */
static idx_t   sieve_cap   = 0;             /* current capacity */

/* Table of k-values where the gap is 4. */
static seq_t  *ks          = NULL;          /* array of ks indices    */
static size_t  ks_len      = 0;             /* number of entries      */
static size_t  ks_cap      = 0;             /* allocated size         */

/* Info to generate nonsquarefree numbers incrementally. */
static idx_t   last_checked = 1;   /* next integer to examine */
static idx_t   nonsq_cnt    = 0;   /* count of nonsquarefree numbers seen so far */
static idx_t   prev_val     = 0;   /* value of previous nonsquarefree number */
static idx_t   prev_idx     = 0;   /* its ordinal */

/* --------------------------------------------------------------------- */
/*  auxiliary functions                                               */
/* --------------------------------------------------------------------- */

/* Extend the sieve so that indices 0 .. limit are valid.  The new sieve
   replaces the old one; the old buffer is freed. */
static void extend_sieve_to(idx_t limit)
{
    if (limit < sieve_cap)
        return;

    /* allocate new sieve buffer */
    uint8_t *buf = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!buf)
        return;      /* out of memory – caller will abort if needed */

    /* ----- compute primes up to sqrt(limit) --------------------------------- */
    idx_t sqrt_limit = (idx_t)floor(sqrt((double)limit)) + 1;
    uint8_t *iscomp  = (uint8_t *)calloc((size_t)(sqrt_limit + 1), sizeof(uint8_t));
    if (!iscomp) {
        free(buf);
        return;
    }

    /* dynamic array for primes */
    size_t   primes_cap = 16;
    uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    size_t   primes_cnt = 0;

    for (idx_t i = 2; i <= sqrt_limit; ++i) {
        if (!iscomp[i]) {
            /* i is prime */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { free(iscomp); free(buf); free(primes); return; }
                primes = tmp;
            }
            primes[primes_cnt++] = i;

            if (i <= sqrt_limit / i) {
                for (idx_t j = i * i; j <= sqrt_limit; j += i)
                    iscomp[j] = 1;
            }
        }
    }

    /* Mark multiples of square primes as nonsquarefree. */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        unsigned __int128 sq128 = (unsigned __int128)p * p;
        if (sq128 > limit)
            break;
        idx_t psq = (idx_t)sq128;
        for (idx_t j = psq; j <= limit; j += psq)
            buf[j] = 1;
    }

    /* Replace the old sieve. */
    free(sieve);
    free(iscomp);
    free(primes);
    sieve      = buf;
    sieve_cap  = limit + 1;
}

/* --------------------------------------------------------------------- */
/*  main sequence routine                                             */
/* --------------------------------------------------------------------- */

/* Return the n-th element of the OEIS sequence A375712. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1 */

    /* Ensure the sieve is initialized. */
    if (sieve_cap == 0)
        extend_sieve_to(10);

    /* Generate terms until we have at least n of them. */
    while (ks_len < (size_t)n) {
        /* Make sure we have sieve information up to the next integer. */
        if (last_checked >= sieve_cap)
            extend_sieve_to(last_checked * 2 + 100);

        /* Is the current integer nonsquarefree? */
        if (sieve[last_checked]) {
            ++nonsq_cnt;   /* new nonsquarefree number found */

            /* Check if the gap from the previous one is 4. */
            if (prev_val != 0 && (last_checked - prev_val) == 4UL) {
                /* Store the index of the previous nonsquarefree number. */
                if (ks_len + 1 > ks_cap) {
                    ks_cap = (ks_cap == 0) ? 16 : ks_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(ks, ks_cap * sizeof(seq_t));
                    if (!tmp) { /* out of memory; aborting is acceptable */
                        abort();
                    }
                    ks = tmp;
                }
                ks[ks_len++] = (seq_t)prev_idx;
            }

            /* Update state for next iteration. */
            prev_val = last_checked;
            prev_idx = nonsq_cnt;
        }

        ++last_checked;
    }

    return ks[n - 1];
}
