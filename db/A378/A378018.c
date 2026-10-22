/*
 * A378018  Primes p which can be written as p = (A060735(k) +- next largest
 *          prime factor not in A060735(k)) for some k.
 * https://oeis.org/A378018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime that can be written as A060735(k) ±
 *            prime(k+1), i.e. primorial(i) * m ± prime(i+1) with 0 < m <
 *            prime(i+1), where i starts at 0 and primorial(i) = ∏_{j=0}^{i-1}
 *            prime(j) (primorial(0)=1)
 * Range:     u8 n <= 18, u16 n <= 43, u32 n <= 46, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: first primes with i=0 give the terms 3,5,7,11,13,17,19,23,29,37 etc. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t) */
#include "../utils/seq_realloc.h"  /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

static seq_t *seq = NULL;       /* sorted list of found primes */
static size_t seq_len = 0;
static size_t seq_cap = 0;

static uint64_t *pr = NULL;     /* list of small primes used for primorial */
static size_t pr_len = 0;
static size_t pr_cap = 0;

static seq_t prim = 1;          /* current primorial = ∏_{j=0}^{iter-1} primes[j] */
static size_t iter = 0;         /* current i (primorial = product of first i primes) */

/* Ensure primes array has at least 'need' primes. */
static void ensure_primes(size_t need)
{
    while (pr_len < need) {
        uint64_t nxt = (pr_len == 0) ? 2u : next_prime(pr[pr_len-1]);

        if (pr_len == pr_cap) {
            size_t new_cap = pr_cap ? pr_cap * 2 : 4;
            uint64_t *tmp = (uint64_t *)realloc(pr, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            pr = tmp;
            pr_cap = new_cap;
        }
        pr[pr_len++] = nxt;
    }
}

/* Grow the seq array at least to 'new_cap' elements. */
static void grow_seq(size_t new_cap)
{
    if (new_cap > seq_cap) {
        seq = seq_realloc(seq, seq_cap, new_cap);
        if (!seq) abort();
        seq_cap = new_cap;
    }
}

/* Insert 'val' into seq keeping the array sorted; skip duplicates. */
static void insert_sorted(seq_t val)
{
    /* binary search for insertion point */
    size_t lo = 0, hi = seq_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (seq[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < seq_len && seq[lo] == val) return;     /* duplicate */

    grow_seq(seq_len + 1);

    /* shift elements to make room */
    for (size_t i = seq_len; i > lo; --i)
        seq[i] = seq[i-1];
    seq[lo] = val;
    ++seq_len;
}

/* ------------------------------------------------------------------ */
/* Sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety: offset is 1 */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* Ensure we have the next prime needed for this step */
        ensure_primes(iter + 1);          /* primes[iter] is prime(i+1) */
        uint64_t nextprime = pr[iter];

        /* For each m with 1 <= m < nextprime: test both ± variations */
        for (uint64_t m = 1; m < nextprime; ++m) {
            seq_t prod = prim * (seq_t)m;

            seq_t cand1 = prod + (seq_t)nextprime;
            if (cand1 > (seq_t)1 && is_prime_big(cand1))
                insert_sorted(cand1);

            seq_t cand2 = prod - (seq_t)nextprime;
            if (cand2 > (seq_t)1 && is_prime_big(cand2))
                insert_sorted(cand2);
        }

        /* Advance primorial and iterator for next round */
        prim *= (seq_t)nextprime;
        ++iter;
    }

    return seq[n - 1];   /* zero‑based index in C, offset 1 in OEIS */
}
