/*
 * A375758  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n > 0, the initial digit of n divides a(n).
 * https://oeis.org/A375758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 227, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: Greedy construction of the lexicographically earliest
 * permutation of positive integers such that for each n>0 the first
 * decimal digit of n divides a(n).  For each new index the smallest
 * unused integer that satisfies the divisibility condition is chosen.
 * The function memoises all results in static arrays so that calls
 * with arbitrary n values are still fast.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);  /* declaration required for other files */

/* ---------- static state --------------------------------------- */

/* Sequence values storage */
static seq_t *seq = NULL;     /* seq[0] = a(1), seq[1] = a(2), ... */
static size_t seq_cap = 0;    /* capacity of seq array */
static idx_t seq_len = 0;     /* number of valid entries in seq */

/* Marker array for used numbers */
static bool *used = NULL;     /* used[i] == true  iff value i has appeared */
static size_t used_cap = 0;   /* capacity of used array */

/* Next unused multiple for each leading digit 1..9 */
static seq_t nextmult[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

/* ---------- helper functions ----------------------------------- */

/* Ensure that used array can be indexed by val (assumes val fits in size_t) */
static void ensure_used_capacity(seq_t val)
{
    size_t idx = (size_t)val;
    if (idx < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 1024;
    if (new_cap <= idx)
        new_cap = idx + 1024;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp)
        abort();
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* Resize seq array to hold at least "len" elements */
static void ensure_seq_capacity(idx_t len)
{
    if ((size_t)len <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < (size_t)len)
        new_cap = (size_t)len;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Return the most significant (leading) decimal digit of n (n>=1) */
static unsigned int first_digit(idx_t n)
{
    while (n >= 10)
        n /= 10;
    return (unsigned int)n;   /* 1 .. 9 */
}

/* ---------- main sequence function ----------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<=0 but return 0 */

    /* Grow the computed part of the sequence if needed */
    while (seq_len < n) {
        seq_len++;                     /* current index, 1‑based */
        unsigned int d = first_digit(seq_len);   /* leading digit 1..9 */

        /* Find the smallest unused multiple of d */
        seq_t val = nextmult[d];
        ensure_used_capacity(val);
        while (used[(size_t)val]) {
            val += d;
            ensure_used_capacity(val);
        }

        /* Store the result */
        ensure_seq_capacity(seq_len);
        seq[seq_len - 1] = val;
        used[(size_t)val] = true;

        /* Advance the next candidate for this digit */
        nextmult[d] = val + d;
    }

    return seq[n - 1];   /* seq[0] corresponds to a(1) */
}
