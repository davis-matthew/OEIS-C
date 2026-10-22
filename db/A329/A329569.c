/*
 * A329569  For all n >= 0, exactly 9 sums are prime among a(n+i) + a(n+j), 0
 *          <= i < j < 6: lexicographically earliest such sequence of distinct
 *          nonnegative numbers.
 * https://oeis.org/A329569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence:
 *   a(0)=0, a(1)=1, a(2)=2, a(3)=5, a(4)=6, a(5)=11
 *   For n>5, a(n) is the smallest integer not appearing earlier such that
 *   the number of primes among the 5 sums
 *       a(n)+a(n-5), a(n)+a(n-4), ..., a(n)+a(n-1)
 *   equals the number of primes among the 5 sums
 *       a(n-6)+a(n-5), a(n-6)+a(n-4), ..., a(n-6)+a(n-1).
 *   The sequence is memoised; calls may appear in any order.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* static storage for the sequence and helper structures */
static seq_t  *seq      = NULL;   /* array of computed terms          */
static size_t  seq_cap  = 0;      /* capacity of seq[]               */
static size_t  seq_sz   = 0;      /* number of terms currently stored*/

static uint8_t *used     = NULL;  /* bitmap: used[v]==1 ⇔ value v already appears */
static size_t  used_cap  = 0;     /* capacity of used[]                */

/* --------------------------------------------------------------------- */
/* Ensure that the used array can index up to val (inclusive).           */
static void ensure_used_capacity(size_t val)
{
    if (val < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap <= val) new_cap <<= 1;
    uint8_t *new_used = (uint8_t *)realloc(used, new_cap);
    if (!new_used) abort();
    memset(new_used + used_cap, 0, new_cap - used_cap);
    used = new_used;
    used_cap = new_cap;
}

/* Ensure seq[] can hold at least n elements.                           */
static void ensure_seq_capacity(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 32;
    while (new_cap < n) new_cap <<= 1;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) abort();
    seq = new_seq;
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* initialise the sequence with the first six terms and mark them used.   */
static void seq_init(void)
{
    ensure_seq_capacity(32);
    seq[0] = 0; seq[1] = 1; seq[2] = 2; seq[3] = 5; seq[4] = 6; seq[5] = 11;
    seq_sz = 6;

    ensure_used_capacity(64);
    memset(used, 0, used_cap);
    for (idx_t i = 0; i < seq_sz; ++i) {
        size_t v = (size_t)seq[i];
        ensure_used_capacity(v);
        used[v] = 1;
    }
}

/* --------------------------------------------------------------------- */
/* public entry point                                                    */
seq_t a(idx_t n)
{
    if (!seq) seq_init();

    /* Simple base cases */
    if (n < (idx_t)seq_sz) return seq[n];

    /* compute terms until index n is reached */
    while ((idx_t)seq_sz <= n) {
        idx_t  k          = seq_sz;          /* new index          */
        idx_t  s_start    = k - 5;           /* start of block    */
        seq_t  prev_term  = seq[k - 6];      /* a(n-6)            */

        /* count primes among a(n-6)+{a(n-5)...a(n-1)} */
        idx_t prev_prime_cnt = 0;
        for (idx_t j = 0; j < 5; ++j) {
            if (is_prime_big(prev_term + seq[s_start + j])) ++prev_prime_cnt;
        }

        /* search smallest unused candidate preserving the count */
        seq_t cand = 0;
        while (1) {
            size_t cand_val = (size_t)cand;
            ensure_used_capacity(cand_val);
            if (used[cand_val]) { ++cand; continue; }

            idx_t cand_cnt = 0;
            for (idx_t j = 0; j < 5; ++j) {
                if (is_prime_big(cand + seq[s_start + j])) ++cand_cnt;
            }
            if (cand_cnt == prev_prime_cnt) break;
            ++cand;
        }

        /* store new term and mark it used          */
        ensure_seq_capacity(seq_sz + 1);
        seq[seq_sz] = cand;

        size_t new_idx = (size_t)cand;
        ensure_used_capacity(new_idx);
        used[new_idx] = 1;

        ++seq_sz;
    }

    return seq[n];
}
