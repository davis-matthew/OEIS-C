/*
 * A351497  Lexicographically earliest infinite sequence of distinct positive
 *          numbers such that, for n>2, a(n) has a common factor with the
 *          second largest previous term but not with the largest previous
 *          term.
 * https://oeis.org/A351497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=3.
   For n>3, a(n) is the smallest unused positive integer x such that
   gcd(x, largest previous term) = 1  and
   gcd(x, second largest previous term) > 1. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t *seq   = NULL;      /* sequence values */
static size_t seq_len  = 0;      /* number of elements stored */
static size_t seq_cap  = 0;      /* allocated capacity */
static seq_t largest   = 0;      /* largest value in seq[0 … seq_len-1] */
static seq_t second_largest = 0;/* second largest value in seq[0 … seq_len-1] */

static unsigned char *used   = NULL; /* bitmap of used numbers */
static size_t used_cap = 0;          /* capacity of used array */

/* Ensure we have enough room to store our sequence. */
static void ensure_seq_cap(void)
{
    if (seq_len < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure the used‑bitmap can hold indices up to *need*. */
static void ensure_used_cap(size_t need)
{
    if (need < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (need >= new_cap) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* Update largest and second largest after adding a new value. */
static void update_maximums(seq_t val)
{
    if (seq_len == 0) {
        largest = val;
        second_largest = 0;
    } else if (val > largest) {
        second_largest = largest;
        largest = val;
    } else if (val > second_largest) {
        second_largest = val;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined below offset */

    /* Lazy initialization of the first three terms */
    if (seq == NULL) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        used_cap = 64;
        used = (unsigned char *)calloc(used_cap, 1);

        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq[2] = (seq_t)3;
        seq_len = 3;

        used[1] = used[2] = used[3] = 1;
        largest = (seq_t)3;
        second_largest = (seq_t)2;
    }

    /* Extend the sequence as far as required. */
    while (seq_len < (size_t)n) {
        seq_t cand;
        seq_t a_largest = largest;
        seq_t a_second   = second_largest;

        for (cand = 1; ; ++cand) {
            size_t idx = (size_t)cand;
            ensure_used_cap(idx + 1);
            if (!used[idx]) {
                if (gcd(cand, a_largest) == (seq_t)1 && gcd(cand, a_second) > (seq_t)1)
                    break;
            }
        }

        ensure_seq_cap();
        seq[seq_len++] = cand;
        used[(size_t)cand] = 1;
        update_maximums(cand);
    }

    return seq[n - 1];
}
