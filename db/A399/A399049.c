/*
 * A399049  a(1) = 1; a(n+1) is the smallest integer greater than a(n) such
 *          that: a(k) + a(k+4) is not a term for any k.
 * https://oeis.org/A399049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A399049:  a(1)=1; a(n+1) = smallest integer > a(n) such that for all k,
   the sum a(k)+a(k+4) is not a term of the sequence. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *seq      = NULL;  /* sequence storage                 */
static size_t  seq_cap = 0;     /* capacity of seq                  */
static idx_t   seq_len = 0;     /* number of terms stored           */

static unsigned char *forbid = NULL;   /* forbidden sums                */
static size_t        forbid_len = 0;   /* size of forbid array          */

static unsigned char *is_seq = NULL;   /* flags for values present       */
static size_t        is_seq_len = 0;   /* size of is_seq array           */

/* Ensure the forbid array can hold index 'val'. */
static void ensure_forbid(seq_t val)
{
    size_t v = (size_t)val;
    if (v >= forbid_len) {
        size_t new_len = forbid_len ? forbid_len * 2 : 64;
        if (new_len <= v) new_len = v + 1;
        unsigned char *tmp = (unsigned char *)realloc(forbid, new_len);
        if (!tmp) exit(EXIT_FAILURE);
        memset(tmp + forbid_len, 0, new_len - forbid_len);
        forbid = tmp;
        forbid_len = new_len;
    }
}

/* Ensure the is_seq array can hold index 'val'. */
static void ensure_is_seq(seq_t val)
{
    size_t v = (size_t)val;
    if (v >= is_seq_len) {
        size_t new_len = is_seq_len ? is_seq_len * 2 : 128;
        if (new_len <= v) new_len = v + 1;
        unsigned char *tmp = (unsigned char *)realloc(is_seq, new_len);
        if (!tmp) exit(EXIT_FAILURE);
        memset(tmp + is_seq_len, 0, new_len - is_seq_len);
        is_seq = tmp;
        is_seq_len = new_len;
    }
}

/* Append a value to the sequence and update the helper tables. */
static void append_term(seq_t value)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = value;

    /* Mark this value as present. */
    ensure_is_seq(value);
    is_seq[(size_t)value] = 1;

    /* When we have at least 5 terms we must forbid the sum involving
       the new term and the term four indices earlier. */
    if (seq_len >= 5) {
        seq_t sum = seq[seq_len - 5] + seq[seq_len - 1];
        ensure_forbid(sum);
        forbid[(size_t)sum] = 1;
    }
}

/* Return the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, return 0 */

    /* Generate terms up to the required index. */
    while (seq_len < n) {
        /* Candidate is the smallest integer greater than the last term. */
        seq_t candidate = (seq_len == 0) ? 1 : seq[seq_len - 1] + 1;

        /* Find the first candidate that is not forbidden and does not
           create a forbidden sum with the term 4 positions earlier. */
        for (;;) {
            size_t cand = (size_t)candidate;

            /* Skip values that are forbidden sums from earlier terms. */
            if (cand < forbid_len && forbid[cand]) {
                candidate++;
                continue;
            }

            /* When a(n-4)+a(n) would equal an existing term, reject. */
            if (seq_len >= 4) {
                seq_t sum = candidate + seq[seq_len - 4];
                size_t sumv = (size_t)sum;
                if (sumv < is_seq_len && is_seq[sumv]) {
                    candidate++;
                    continue;
                }
            }
            break;  /* candidate accepted */
        }

        append_term(candidate);
    }

    return seq[n - 1];
}
