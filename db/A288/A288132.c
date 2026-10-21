/*
 * A288132  Fixed point of the mapping 00->0010, 1->11, starting with 00.
 * https://oeis.org/A288132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth term of the infinite binary word obtained by iterating
   the substitution:
        00 → 0010
        1  → 11
   starting with the word "00".
   The sequence is 0‑1 valued, a(1)=0, a(2)=0, a(3)=1, … */

#include <stddef.h>
#include <stdlib.h>

static unsigned char *seq_mem = NULL;    /* stores the generated word */
static size_t          seq_len = 0;       /* current length */
static size_t          seq_cap = 0;       /* allocated capacity */

/* ensure seq_mem can hold at least 'min_cap' elements */
static void ensure_capacity(size_t min_cap)
{
    if (seq_cap >= min_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < min_cap)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(seq_mem, new_cap);
    if (!tmp)          /* allocation failed – terminate */
        exit(1);
    seq_mem = tmp;
    seq_cap = new_cap;
}

/* generate the word until it has at least 'target' symbols */
static void grow_sequence(idx_t target)
{
    if (seq_len == 0) {          /* initialise with the seed "00" */
        ensure_capacity(4);
        seq_mem[0] = 0;           /* a(1) */
        seq_mem[1] = 0;           /* a(2) */
        seq_len = 2;
    }

    while (seq_len < target) {
        size_t i = 0, new_len = 0, j = 0;
        /* first pass: count length of the next word */
        while (i < seq_len) {
            if (seq_mem[i] == 0 && i + 1 < seq_len && seq_mem[i + 1] == 0) {
                new_len += 4;      /* "0010" */
                i += 2;
            } else if (seq_mem[i] == 1) {
                new_len += 2;      /* "11" */
                i += 1;
            } else {
                new_len += 1;      /* single "0" */
                i += 1;
            }
        }

        /* second pass: actually build the next word */
        unsigned char *next = (unsigned char *)malloc(new_len);
        if (!next) exit(1);
        i = 0; j = 0;
        while (i < seq_len) {
            if (seq_mem[i] == 0 && i + 1 < seq_len && seq_mem[i + 1] == 0) {
                /* append "0010" */
                next[j++] = 0;
                next[j++] = 0;
                next[j++] = 1;
                next[j++] = 0;
                i += 2;
            } else if (seq_mem[i] == 1) {
                /* append "11" */
                next[j++] = 1;
                next[j++] = 1;
                i += 1;
            } else {
                /* single "0" stays unchanged */
                next[j++] = 0;
                i += 1;
            }
        }

        free(seq_mem);
        seq_mem = next;
        seq_len = new_len;
        seq_cap = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n<1, but return 0 safely */
        return (seq_t)0;

    /* generate enough of the word to answer this query */
    grow_sequence((size_t)n);

    /* a(n) corresponds to seq_mem[n-1] (0‑based indexing) */
    return (seq_t)seq_mem[n - 1];
}
