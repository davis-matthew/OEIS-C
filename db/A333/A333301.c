/*
 * A333301  a(1) = 1, a(2) = 2. For n>2, if a(n-1) is odd, a(n) = a(n-1) +
 *          a(n-2), and otherwise a(n) is the smallest missing number.
 * https://oeis.org/A333301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A333301: a(1)=1, a(2)=2; for n>2, 
   if a(n‑1) is odd then a(n)=a(n‑1)+a(n‑2), otherwise a(n)=smallest missing number. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Memoization of the sequence and of the seen numbers */
static seq_t *seq     = NULL;   /* array of produced sequence values   */
static size_t seq_len = 0;      /* number of terms stored so far       */
static size_t seq_cap = 0;      /* current capacity of seq array       */

static unsigned char *seen     = NULL; /* bitmap of values that have appeared */
static size_t seen_cap = 0;           /* capacity of seen bitmap           */

static seq_t next_missing = 1;        /* smallest missing number so far    */

static void ensure_seq_capacity(size_t n)
{
    if (seq_len >= n) return;
    size_t new_cap = seq_cap ? seq_cap : 2;
    while (new_cap < n) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) abort();
    seq_cap = new_cap;
}

static void ensure_seen_capacity(size_t v)
{
    if (v < seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap : 1;
    while (new_cap <= v) new_cap <<= 1;
    seen = (unsigned char *)realloc(seen, new_cap);
    if (!seen) abort();
    for (size_t i = seen_cap; i < new_cap; ++i) seen[i] = 0;
    seen_cap = new_cap;
}

static void update_next_missing(void)
{
    /* Increment next_missing until we find an unseen number.
       If next_missing exceeds seen_cap, all numbers beyond are unseen. */
    while (next_missing < (seq_t)seen_cap && seen[(size_t)next_missing]) {
        ++next_missing;
    }
}

seq_t a(idx_t n)
{
    /* index n is 1‑based; behaviour for n<1 is undefined, return 0. */
    if (n < 1) return 0;

    size_t nn = (size_t)n;

    /* Resize seq array if needed */
    ensure_seq_capacity(nn);

    /* Compute terms up to n */
    for (size_t i = seq_len + 1; i <= nn; ++i) { /* i is 1‑based */
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)2;
        } else {
            seq_t prev = seq[i - 2];      /* a(i-1) */
            if (prev & 1) {               /* odd */
                val = prev + seq[i - 3];  /* a(i-2) */
            } else {                     /* even */
                val = next_missing;
            }
        }
        seq[i - 1] = val;  /* store */

        /* Mark value as seen */
        size_t vidx = (size_t)val;
        ensure_seen_capacity(vidx);
        seen[vidx] = 1;

        /* Update next_missing */
        update_next_missing();
    }

    seq_len = nn;
    return seq[nn - 1];
}
