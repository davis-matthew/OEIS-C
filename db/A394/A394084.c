/*
 * A394084  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest unused
 *          positive integer k whose greatest prime factor matches the
 *          greatest prime factor of the sum of the greatest prime factors of
 *          the previous terms.
 * https://oeis.org/A394084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2, let S = Σ_{i=1}^{n-1} gpf(a(i)) * where
 *            gpf(1)=1, gpf(m) is the largest prime factor of m. * Then a(n)
 *            is the smallest positive integer k not already used * such that
 *            gpf(k)=gpf(S)
 * Range:     u8 n <= 72, u16 n <= 4368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------------*
 *  Include utilities
 *------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gpf.h"

/*------------------------------------------------------------------*
 *  Internal state for memoisation
 *------------------------------------------------------------------*/
static seq_t *seq = NULL;           /* array of computed terms          */
static size_t seq_cap = 0;          /* current capacity of seq array    */
static idx_t  seq_len = 0;          /* number of terms already stored   */
static uint64_t sum_gpf = 0;        /* Σ gpf(a(i)) for i < seq_len      */

static bool *used = NULL;           /* bit‑set of used positives         */
static size_t used_cap = 0;         /* capacity of used bit‑set          */

static uint64_t *next_mult = NULL;  /* for each prime p: next multiple  */
static size_t next_mult_cap = 0;    /* capacity of next_mult array       */

/*------------------------------------------------------------------*
 *  Utility: ensure seq array big enough
 *------------------------------------------------------------------*/
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;

    seq_t *newseq = (seq_t*)malloc(newcap * sizeof(seq_t));
    if (!newseq) exit(1);

    if (seq) {
        memcpy(newseq, seq, seq_len * sizeof(seq_t));
        free(seq);
    }
    seq = newseq;
    seq_cap = newcap;
}

/*------------------------------------------------------------------*
 *  Utility: ensure used array big enough to index 'val'
 *------------------------------------------------------------------*/
static void ensure_used_cap(uint64_t val)
{
    if (val + 1 <= used_cap) return;
    size_t newcap = used_cap ? used_cap * 2 : 64;
    while (newcap <= val) newcap <<= 1;

    bool *newused = (bool*)malloc(newcap * sizeof(bool));
    if (!newused) exit(1);
    memset(newused, 0, newcap * sizeof(bool));

    if (used) {
        memcpy(newused, used, used_cap * sizeof(bool));
        free(used);
    }
    used = newused;
    used_cap = newcap;
}

/*------------------------------------------------------------------*
 *  Utility: ensure next_mult array big enough for index p
 *------------------------------------------------------------------*/
static void ensure_prime_cap(size_t p)
{
    if (p < next_mult_cap) return;
    size_t newcap = next_mult_cap ? next_mult_cap * 2 : 16;
    while (newcap <= p) newcap <<= 1;

    uint64_t *newp = (uint64_t*)malloc(newcap * sizeof(uint64_t));
    if (!newp) exit(1);
    memset(newp, 0, newcap * sizeof(uint64_t));

    if (next_mult) {
        memcpy(newp, next_mult, next_mult_cap * sizeof(uint64_t));
        free(next_mult);
    }
    next_mult = newp;
    next_mult_cap = newcap;
}

/*------------------------------------------------------------------*
 *  Initialise with first two terms
 *------------------------------------------------------------------*/
static void init_sequence(void)
{
    if (seq_len > 0) return;   /* already initialised */

    /* Allocate space for two terms */
    ensure_seq_cap(2);
    seq[0] = (seq_t)1;
    seq[1] = (seq_t)2;
    seq_len = 2;
    sum_gpf = 1 + 2;           /* gpf(1)=1, gpf(2)=2   */

    /* Mark 1 and 2 as used */
    ensure_used_cap(2);
    used[1] = true;
    used[2] = true;
}

/*------------------------------------------------------------------*
 *  Main function
 *------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety, index 0 not defined */

    size_t target_n = (size_t)n;

    init_sequence();

    while (seq_len < target_n)
    {
        uint64_t target = gpf(sum_gpf);      /* required gpf for next term */

        /* Ensure we can store next_multiple for this prime */
        ensure_prime_cap((size_t)target);
        uint64_t cand = next_mult[target];
        if (cand == 0) cand = target;       /* first multiple of the prime */

        while (true)
        {
            ensure_used_cap(cand);
            if (!used[cand] && gpf(cand) == target)
                break;                       /* found the smallest unused */
            cand += target;                  /* jump to next multiple of prime */
        }

        /* Record the chosen number */
        used[cand] = true;
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = (seq_t)cand;

        /* Update state for the next iteration */
        sum_gpf += target;
        seq_len++;

        /* Advance the next multiple pointer for this prime */
        next_mult[target] = cand + target;
    }

    return seq[n - 1];
}
