/*
 * A304802  Solution (b(n)) of the complementary equation a(n) = b(n) + b(3n);
 *          see Comments.
 * https://oeis.org/A304802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A304802 – complementary sequence b(n) defined recursively:
   b(n) = least unused positive integer;
   a(n) = b(n) + b(3n);
   b(n) is returned by a(n).  The sequence is computed by
   iteratively assigning the smallest unused integer to b(n) and
   marking both that integer and the corresponding a(n/3) as used. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static seq_t *b = NULL;          /* array of b(n) values            */
static size_t b_cap   = 0;        /* capacity of b array             */
static size_t b_size  = 0;        /* number of values actually stored */

static uint8_t *used = NULL;     /* bitmap of already used integers */
static size_t   used_cap = 0;    /* capacity of used array          */

static uint32_t next_candidate = 1; /* next integer to test for b(n)   */

/* Ensure that 'used' can hold indices up to 'need' (inclusive).  */
static void enlarge_used(size_t need)
{
    if (need < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 1;
    while (new_cap <= need)
        new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
    if (!tmp)
        exit(EXIT_FAILURE);
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* Ensure that 'b' can hold indices up to 'need' (inclusive).  */
static void enlarge_b(size_t need)
{
    if (need < b_cap)
        return;
    size_t new_cap = b_cap ? b_cap : 1;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    b = tmp;
    b_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Compute b(0) … b(n) if they do not yet exist.  */
    if (b_size <= (size_t)n) {
        size_t need = (size_t)n;
        enlarge_b(need);
        for (size_t i = b_size; i <= need; ++i) {
            /* Find smallest unused positive integer for b(i). */
            while (1) {
                if ((size_t)next_candidate >= used_cap)
                    enlarge_used((size_t)next_candidate);
                if (!used[(size_t)next_candidate])
                    break;
                ++next_candidate;
            }
            seq_t val = (seq_t)next_candidate;
            b[i] = val;
            used[(size_t)next_candidate] = 1;      /* mark b(i) as used            */
            ++next_candidate;

            /* If i is a multiple of 3, compute a(i/3)=b(i/3)+b(i)
               and mark it as used as well.                  */
            if (i % 3 == 0) {
                idx_t k = i / 3;
                seq_t a_val = b[k] + b[i];
                if ((size_t)a_val >= used_cap)
                    enlarge_used((size_t)a_val);
                used[(size_t)a_val] = 1;
            }
        }
        b_size = need + 1;
    }
    return b[(size_t)n];
}
