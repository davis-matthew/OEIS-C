/*
 * A372704  a(1)=1; a(2)=2; for n>1, this is the lexicographically earliest
 *          sequence such that, following each occurrence of a(n), a(n) is
 *          banned for the next k terms, where k is the number of terms prior
 *          to a(n) that are not equal to a(n).
 * https://oeis.org/A372704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A372704: lexicographically earliest sequence with bans.
   Rule: after each occurrence of a value v, v is banned for the next
   k terms, where k = (#terms before that occurrence that are not equal to v).
   a(1)=1, a(2)=2.  This implementation builds the sequence incrementally
   and memoises it in static arrays. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for the sequence values */
static seq_t *seq_cache = NULL;     /* array of sequence terms           */
static size_t seq_len  = 0;         /* number of terms stored            */
static size_t seq_cap  = 0;         /* capacity of seq_cache             */

/* static tables for per‑value bookkeeping */
static idx_t *cnt    = NULL;        /* occurrences of each value         */
static idx_t *expire = NULL;        /* first position where value is no
                                      longer banned                      */
static size_t val_cap = 0;          /* capacity for cnt/expire arrays    */

/* ensure seq_cache has at least min_cap entries */
static void grow_seq_cache(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    seq_cache = p;
    seq_cap   = new_cap;
}

/* ensure cnt/expire have at least min_cap entries (indices 0 .. min_cap-1) */
static void grow_val_tables(size_t min_cap)
{
    if (val_cap >= min_cap) return;
    size_t new_cap = val_cap ? val_cap : 64;
    while (new_cap < min_cap) new_cap <<= 1;
    idx_t *pcnt    = (idx_t *)realloc(cnt,    new_cap * sizeof(idx_t));
    idx_t *p_expire= (idx_t *)realloc(expire, new_cap * sizeof(idx_t));
    if (!pcnt || !p_expire) abort();
    /* initialise new entries */
    for (size_t i = val_cap; i < new_cap; ++i) {
        pcnt[i]    = 0;
        p_expire[i]= 1;                /* never banned initially          */
    }
    cnt    = pcnt;
    expire = p_expire;
    val_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for 0; guard          */

    /* initialise data structures on first call */
    if (seq_len < 2) {
        grow_seq_cache(2);
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        seq_len = 2;

        grow_val_tables(3);             /* indices 0..2 are needed         */
        cnt[1]    = 1;
        expire[1] = 2;                   /* 1 banned until position 2       */
        cnt[2]    = 1;
        expire[2] = 4;                   /* 2 banned until position 4       */
    }

    /* extend the sequence up to the requested index n */
    for (idx_t pos = seq_len + 1; pos <= n; ++pos) {
        /* find the smallest v that is not banned at position pos */
        idx_t v = 1;
        while (true) {
            if ((idx_t)v >= val_cap) grow_val_tables(v + 1);
            if (expire[v] <= pos) break;
            ++v;
        }

        /* compute ban length k for this occurrence of v */
        idx_t prev_cnt = cnt[v];
        idx_t k = (pos - 1) - prev_cnt;

        /* update occurrence count and expiry position */
        cnt[v] = prev_cnt + 1;
        idx_t new_expire = pos + k + 1;
        if (new_expire > expire[v]) expire[v] = new_expire;

        /* store the term */
        grow_seq_cache(seq_len + 1);
        seq_cache[seq_len] = (seq_t)v;
        ++seq_len;
    }

    return seq_cache[n - 1];
}
