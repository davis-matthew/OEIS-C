/*
 * A352877  Lexicographically earliest sequence of distinct nonnegative terms
 *          such that two consecutive terms can be added without carries in
 *          base 3.
 * https://oeis.org/A352877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   no_carry
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352877: lexicographically earliest permutation of
   nonnegative integers such that a(n)+a(n+1) has no carries in base 3. */
/* The sequence is generated greedily: a(0)=0 and for n>0, a(n) is the
   smallest unused integer such that no carry occurs when adding a(n-1) and a(n)
   in base 3. This definition is implemented directly. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/no_carry.h"

/* Dynamic containers for the generated sequence and the used‑number set. */
static seq_t *seq_buf          = NULL;   /* buffer of a(n) values          */
static size_t seq_len          = 0;      /* number of values computed      */
static size_t seq_cap          = 0;      /* current capacity of seq_buf    */

static unsigned char *used_buf = NULL;   /* bitmask of numbers already used */
static size_t used_cap         = 0;      /* capacity of used_buf in bytes   */

/* Ensure used_buf can index the number 'v'.  Used numbers are all
   0‑based, so the buffer is grown when necessary. */
static void ensure_used_capacity(seq_t v)
{
    /* The value v must fit in a size_t for indexing.  We assume it does
       for all reasonable n (≤10⁴). */
    size_t idx = (size_t)v;
    if (idx < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap << 1 : 1;
    while (new_cap <= idx)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used_buf, new_cap);
    if (!tmp) abort();          /* out‑of‑memory */
    for (size_t i = used_cap; i < new_cap; ++i)
        tmp[i] = 0;
    used_buf = tmp;
    used_cap = new_cap;
}

/* Ensure seq_buf can hold at least n+1 elements. */
static void ensure_seq_capacity(idx_t n)
{
    if ((size_t)n + 1 <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_buf = tmp;
    seq_cap = new_cap;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (seq_len == 0) {
        ensure_seq_capacity(0);
        seq_buf[0] = (seq_t)0;
        seq_len = 1;
        ensure_used_capacity(0);
        used_buf[0] = 1;
    }

    /* Extend the sequence up to index n if necessary. */
    if ((idx_t)(seq_len - 1) < n) {
        for (idx_t i = seq_len; i <= n; ++i) {
            seq_t prev = seq_buf[i - 1];
            seq_t cand = 0;

            for (;;) {
                /* Make sure the used table can index cand. */
                ensure_used_capacity(cand);
                /* Skip if already used. */
                if (!used_buf[(size_t)cand] && no_carry(prev, cand))
                    break;      /* cand chosen */
                ++cand;
            }

            ensure_seq_capacity(i);
            seq_buf[i] = cand;
            used_buf[(size_t)cand] = 1;
            seq_len = (size_t)i + 1;
        }
    }

    return seq_buf[n];
}
