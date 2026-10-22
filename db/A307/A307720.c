/*
 * A307720  Lexicographically earliest sequence of positive integers in which,
 *          for all positive k, there are exactly k pairs of consecutive terms
 *          whose product is k.
 * https://oeis.org/A307720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, nice
 * Formula:   Lexicographically earliest sequence of positive integers such
 *            that for each k ≥ 1 exactly k consecutive pairs have product k
 *            (OEIS A307720)
 * Range:     u8 n <= 9445, u16 n <= 9445, u32 n <= 9445, u64 n <= 9445,
 *            u128 n <= 9445, big any n
 * Helpers:   seq_alloc_cap, seq_realloc_cap, seq_setzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/seq_alloc_cap.h"
#include "../utils/seq_realloc_cap.h"
#include "../utils/seq_setzero.h"

/* Helpers that work with both C integer types and C++ big‑integer classes */

/* Static state shared by all calls to a(n) */
static seq_t *seq   = NULL;   /* sequence values a(1), a(2), …          */
static size_t seq_len = 0;    /* number of stored terms                 */
static size_t seq_cap = 0;    /* capacity of seq array                  */

static seq_t *cnt   = NULL;   /* counts of pairs for each product       */
static size_t cnt_cap = 0;    /* capacity of cnt array                  */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined offset */

    /* initialise on first invocation */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = seq_alloc_cap(seq_cap);
        seq[0] = (seq_t)1;            /* a(1) = 1 */
        seq_len = 1;

        cnt_cap = 16;
        cnt = seq_alloc_cap(cnt_cap);
        seq_setzero(cnt, 0, cnt_cap);
    }

    /* grow the sequence until we have term n */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        seq_t x = (seq_t)1;

        for (;;) {
            seq_t prod = prev * x;          /* product of a(n) * a(n+1) */
            idx_t prod_idx = (idx_t)prod;   /* index in the cnt array */

            /* enlarge cnt array if necessary */
            if ((size_t)prod_idx >= cnt_cap) {
                size_t new_cap = cnt_cap ? cnt_cap << 1 : 16;
                while (new_cap <= (size_t)prod_idx) new_cap <<= 1;
                seq_t *new_cnt = seq_realloc_cap(cnt, cnt_cap, new_cap);
                seq_setzero(new_cnt, cnt_cap, new_cap);
                cnt = new_cnt;
                cnt_cap = new_cap;
            }

            /* check if this product still needs more occurrences */
            if (cnt[prod_idx] < prod) {
                cnt[prod_idx]++;           /* record this occurrence   */

                /* enlarge sequence array if needed */
                if (seq_len >= seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                    seq_t *new_seq = seq_realloc_cap(seq, seq_cap, new_cap);
                    seq = new_seq;
                    seq_cap = new_cap;
                }

                seq[seq_len] = x;           /* accept this term         */
                seq_len++;
                break;                      /* ready for next term      */
            }
            x++;                            /* try next candidate value */
        }
    }

    return seq[n - 1];
}
