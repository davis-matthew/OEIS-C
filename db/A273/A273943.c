/*
 * A273943  Put and say how often: a(n) = n/2 if n is even, else a(n) = number
 *          of matches of "a(n-1)" in the concatenation of all "a(i)" with i <
 *          n, where "x" denotes the decimal string of x.
 * https://oeis.org/A273943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n/2 if n is even * a(n) = number of occurrences of the
 *            decimal string of a(n-1) * in the concatenated decimal strings
 *            of a(i) for i < n
 * Range:     u8 n <= 510, u16 n <= 8781, u32 n <= 8781, u64 n <= 8781,
 *            u128 n <= 8781, big any n
 * Helpers:   count_occurrences
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  The implementation keeps a cache of already computed terms
 * and a growing buffer with the concatenation of all terms seen so
 * far.  The cache allows calls in any order.  All arithmetic
 * involving seq_t uses only the permitted operators.  The decimal
 * conversion works for arbitrary‑precision seq_t types as well as
 * the builtin unsigned integer types. */

#include <stdlib.h>
#include <string.h>
#include "../utils/count_occurrences.h"

static size_t seq_to_dec(seq_t x, char *out)
{
    if (x == 0) {
        out[0] = '0';
        return 1;
    }
    char tmp[512];
    size_t idx = 0;
    while (x != 0) {
        int d = (int)(x % (seq_t)10);
        tmp[idx++] = '0' + d;
        x /= (seq_t)10;
    }
    for (size_t i = 0; i < idx; ++i)
        out[i] = tmp[idx - 1 - i];
    return idx;
}

static void ensure_capacity(size_t need, size_t *cap, void **ptr,
                            size_t elem_size)
{
    if (need <= *cap) return;
    size_t newcap = (*cap > 0 ? *cap : 1);
    while (newcap < need) newcap <<= 1;
    *ptr = realloc(*ptr, newcap * elem_size);
    *cap = newcap;
}

static void ensure_array_cap(seq_t **arr, size_t *cap, size_t need)
{
    ensure_capacity(need, cap, (void**)arr, sizeof(seq_t));
}

static void ensure_text_cap(char **buf, size_t *cap, size_t need)
{
    ensure_capacity(need, cap, (void**)buf, sizeof(char));
}

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *seq   = NULL;
    static size_t seq_cap   = 0;
    static idx_t next_computed = 0;  /* first index not yet computed */

    /* Static buffer holding concatenated decimal string of all
       computed terms.  text_len is the current used length. */
    static char *text = NULL;
    static size_t text_cap = 0;
    static size_t text_len = 0;

    /* Initialise on first call. */
    if (seq == NULL) {
        seq_cap = 128;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 0;                 /* a(0) = 0 */
        next_computed = 1;

        text_cap = 256;
        text = (char*)malloc(text_cap * sizeof(char));
        text[0] = '0';
        text_len = 1;
    }

    /* Grow the sequence array to fit the requested n. */
    ensure_array_cap(&seq, &seq_cap, (size_t)n + 1);

    /* Compute terms up to n if required. */
    for (idx_t i = next_computed; i <= n; ++i) {
        seq_t val;
        if ((i & 1) == 0) {           /* even index */
            val = (seq_t)i / 2;
        } else {                      /* odd index */
            char pat[512];
            size_t pat_len = seq_to_dec(seq[i-1], pat);
            size_t occ = count_occurrences(text, text_len, pat, pat_len);
            val = (seq_t)occ;
        }

        /* Append the decimal string of val to the accumulated text. */
        char numbuf[512];
        size_t num_len = seq_to_dec(val, numbuf);
        size_t new_len = text_len + num_len;
        ensure_text_cap(&text, &text_cap, new_len);
        memcpy(text + text_len, numbuf, num_len);
        text_len = new_len;

        seq[i] = val;
    }

    next_computed = n + 1;
    return seq[n];
}
