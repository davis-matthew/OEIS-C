/*
 * A297418  a(n) is the smallest positive number not yet in the sequence that
 *          contains the largest digit in a(n-1); a(1)=0.
 * https://oeis.org/A297418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A297418:
   a(1) = 0.
   For n>1, a(n) is the smallest positive integer not yet in the sequence
   that contains the largest decimal digit of a(n-1).
   The sequence is memoised so that calls arrive in any order. */

#include <stdlib.h>
#include <stdint.h>

/* dynamic storage for the sequence */
static seq_t *seq   = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* bitmap of used numbers: 1 = used, 0 = unused */
static uint8_t *used = NULL;
static size_t used_cap = 0;

/* next candidate to test for each possible digit (0‑9) */
static idx_t next_candidate[10] = {0};

/*-------------------- utilities ---------------------------------------*/
static void grow_seq(void)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        seq_t *p = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!p) exit(1);             /* fatal error */
        seq = p;
        seq_cap = new_cap;
    }
}

static void ensure_used(idx_t idx)
{
    size_t need = (size_t)idx + 1;
    if (need > used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 8;
        while (new_cap < need) new_cap <<= 1;
        uint8_t *p = (uint8_t *)realloc(used, new_cap);
        if (!p) exit(1);          /* fatal error */
        for (size_t i = used_cap; i < new_cap; ++i) p[i] = 0;
        used = p;
        used_cap = new_cap;
    }
}

/* largest decimal digit in v (0‑9) */
static int largest_digit(seq_t v)
{
    if (v == 0) return 0;
    int maxd = 0;
    while (v > 0) {
        int d = (int)(v % 10);
        if (d > maxd) maxd = d;
        v /= 10;
    }
    return maxd;
}

/* does v contain the decimal digit d? */
static int contains_digit(seq_t v, int d)
{
    if (v == 0) return d == 0;
    while (v > 0) {
        if ((int)(v % 10) == d) return 1;
        v /= 10;
    }
    return 0;
}

/*--------------------- main function ----------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */

    /* initialise first term a(1)=0 */
    if (seq_len == 0) {
        grow_seq();
        seq[seq_len++] = 0;
        ensure_used(0);
        used[0] = 1;
    }

    /* extend the cache up to index n */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        int d = largest_digit(prev);

        /* find smallest unused positive integer containing digit d */
        idx_t cand = next_candidate[d];
        if (cand == 0) cand = 1;           /* avoid 0 for positive integers */

        for (;;) {
            ensure_used(cand);
            if (!used[(size_t)cand] && contains_digit((seq_t)cand, d))
                break;
            ++cand;
        }

        /* store the new term */
        grow_seq();
        seq[seq_len++] = (seq_t)cand;

        /* mark used and advance the next candidate pointer */
        used[(size_t)cand] = 1;
        next_candidate[d] = cand + 1;
    }

    return seq[n - 1];
}
