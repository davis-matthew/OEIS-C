/*
 * A297293  Solution (c(n)) of the system of 3 complementary equations in
 *          Comments.
 * https://oeis.org/A297293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the c(n) sequence from the complementary system:
 *   a(n) = least unused positive integer;
 *   b(n) = least unused integer ≥ a(n)+2;
 *   c(n) = a(n)+b(n)-2.
 * The sequence a(0)=2, a(1)=8, … is the OEIS A297293. 
 * No closed formula is known; this code builds the terms incrementally,
 * memoizing previous values so that calls may arrive in any order. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Forward declarations of helper functions */
static void ensure_seq_capacity(idx_t idx);
static void ensure_used_capacity(idx_t idx);
static void update_mex(void);
static idx_t find_next_unused(idx_t start);

/* Static data for memoisation */
static seq_t *a_seq = NULL, *b_seq = NULL, *c_seq = NULL;
static size_t seq_cap = 0;          /* capacity of the seq arrays   */
static size_t seq_len = 0;          /* number of terms computed so far      */

static unsigned char *used = NULL;  /* bitmap of numbers used                */
static size_t used_cap = 0;         /* capacity of used bitmap               */
static idx_t mex = 4;               /* next candidate for a(n)              */

/* Ensure the seq arrays can hold at least (idx+1) elements. */
static void ensure_seq_capacity(idx_t idx)
{
    if ((size_t)idx >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        while (new_cap <= (size_t)idx) new_cap <<= 1;
        seq_t *na = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
        seq_t *nb = (seq_t *)realloc(b_seq, new_cap * sizeof(seq_t));
        seq_t *nc = (seq_t *)realloc(c_seq, new_cap * sizeof(seq_t));
        if (!na || !nb || !nc) abort();          /* memory exhausted */
        a_seq = na; b_seq = nb; c_seq = nc;
        seq_cap = new_cap;
    }
}

/* Ensure the used bitmap can address index 'idx'. */
static void ensure_used_capacity(idx_t idx)
{
    if ((size_t)idx >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 64;
        while (new_cap <= (size_t)idx) new_cap <<= 1;
        unsigned char *nu = (unsigned char *)realloc(used, new_cap);
        if (!nu) abort();
        memset(nu + used_cap, 0, new_cap - used_cap);
        used = nu;
        used_cap = new_cap;
    }
}

/* Move mex to the next unused integer. */
static void update_mex(void)
{
    ensure_used_capacity(mex);
    while (mex < used_cap && used[mex]) ++mex;
}

/* Find first unused integer ≥ start. */
static idx_t find_next_unused(idx_t start)
{
    idx_t cand = start;
    while (true) {
        ensure_used_capacity(cand);
        if (!used[cand]) return cand;
        ++cand;
    }
}

/* Initialise static data the first time the function is called. */
static void init(void)
{
    ensure_seq_capacity(0);            /* allocate initial seq capacity */
    a_seq[0] = (seq_t)1;               /* a(0) = 1   */
    b_seq[0] = (seq_t)3;               /* b(0) = 3   */
    c_seq[0] = (seq_t)2;               /* c(0) = 2   */
    seq_len = 1;

    /* initialise used bitmap, mark 1,2,3 as used */
    ensure_used_capacity(3);
    used[1] = used[2] = used[3] = 1;
    /* set mex to the next unused after 3 */
    mex = 4;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (!a_seq) init();              /* lazy initialisation */

    /* extend precomputation up to n if necessary */
    if ((size_t)n >= seq_len) {
        for (idx_t idx = (idx_t)seq_len; idx <= n; ++idx) {
            /* a(idx): smallest unused positive integer */
            seq_t aval = (seq_t)mex;
            ensure_used_capacity((idx_t)aval);
            used[(idx_t)aval] = 1;
            update_mex();

            /* b(idx): first unused ≥ a(idx)+2 */
            idx_t start = (idx_t)(aval + (seq_t)2);
            idx_t b_candidate = find_next_unused(start);
            seq_t bval = (seq_t)b_candidate;
            ensure_used_capacity(b_candidate);
            used[b_candidate] = 1;
            update_mex();

            /* c(idx): a(idx)+b(idx)-2 */
            seq_t cval = aval + bval - (seq_t)2;
            idx_t c_candidate = (idx_t)cval;
            ensure_used_capacity(c_candidate);
            used[c_candidate] = 1;
            update_mex();

            /* store the triple */
            ensure_seq_capacity(idx);
            a_seq[idx] = aval;
            b_seq[idx] = bval;
            c_seq[idx] = cval;
        }
        seq_len = (size_t)n + 1;
    }

    return c_seq[n];
}
