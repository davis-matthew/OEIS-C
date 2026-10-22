/*
 * A326106  Lexicographically earliest sequence of distinct terms such that
 *          a(n) is not divisible by any digit of a(n+1).
 * https://oeis.org/A326106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 1422, u32 n <= 1422, u64 n <= 1422,
 *            u128 n <= 1422, big any n
 * Helpers:   prev_divides_by_digit_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence of distinct integers a(n) where
   a(n) is not divisible by any digit of a(n+1).  The sequence is
   generated greedily: a(1)=1 and a(k) (k>1) is the smallest unused
   positive integer such that for every non‑zero digit d of a(k) we have
   a(k-1) % d != 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/prev_divides_by_digit_of.h"

/*--- internal state ---------------------------------------------*/
static seq_t *seq = NULL;           /* array of sequence terms */
static size_t seq_cap = 0;          /* allocated capacity for seq     */
static size_t seq_len = 0;          /* number of terms already stored */

static unsigned char *used = NULL;  /* bitmap of used numbers (0/1)  */
static size_t used_cap = 0;         /* allocated capacity for used   */

/* Ensure seq has space for 'needed' elements */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure used has space for indexes up to 'needed'-1 */
static void ensure_used_cap(size_t needed)
{
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (tmp) {
        /* zero initialise the new part */
        size_t old = used_cap;
        memset(tmp + old, 0, new_cap - old);
    }
    used = tmp;
    used_cap = new_cap;
}

/*-----------------------------------------------------------------*/
/*   Main sequence function                                         */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined but harmless */

    /* return stored value if already computed */
    if ((size_t)n <= seq_len) return seq[(size_t)n - 1];

    /* generate terms up to index n */
    size_t target = (size_t)n;
    for (size_t i = seq_len; i < target; ++i)
    {
        seq_t val;

        if (i == 0) {                     /* a(1) = 1 */
            val = 1;
        }
        else {
            seq_t prev = seq[i - 1];
            seq_t cand = 1;

            /* find smallest unused integer satisfying condition */
            while (true) {
                size_t idx = (size_t)cand;
                if (idx >= used_cap) ensure_used_cap(idx + 1);
                if (!used[idx] && !prev_divides_by_digit_of(prev, cand)) {
                    val = cand;
                    break;
                }
                ++cand;
            }
        }

        /* store the new term */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = val;

        /* mark as used */
        size_t idx = (size_t)val;
        if (idx >= used_cap) ensure_used_cap(idx + 1);
        used[idx] = 1;
    }

    return seq[target - 1];
}
