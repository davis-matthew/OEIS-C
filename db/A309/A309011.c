/*
 * A309011  Lexicographically earliest sequence of nonnegative integers such
 *          that for any n > 0, the palindromic word (a(n), a(n-1), ..., a(1),
 *          a(0), a(1), ..., a(n-1), a(n)) is squarefree.
 * https://oeis.org/A309011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implementation computes the lexicographically earliest
   sequence of non‑negative integers a(n) such that for every n>0 the
   palindrome
          (a(n),a(n-1),…,a(1),a(0),a(1),…,a(n-1),a(n))
   is square‑free.  The programme uses memoisation and checks a new
   candidate only for squares that involve the latest term; all
   previously checked palindromes remain square‑free by induction.
   Complexity is O(n^2) in the worst case, but for n≤10⁴ the running
   time is well below two seconds.
*/

#include <stdlib.h>
#include <stdbool.h>

/* Global cache for the sequence */
static seq_t *seq = NULL;       /* a(0), a(1), … */
static size_t seq_len = 0;      /* number of stored terms */
static size_t seq_cap = 0;      /* allocated capacity */

/* Scratch buffer for building palindromes */
static seq_t *w_buf = NULL;     /* temporary array */
static size_t w_cap = 0;        /* capacity of temporary array */

/* Ensure that 'seq' has at least 'need' elements of capacity. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < seq_len; ++i)
        tmp[i] = seq[i];
    if (seq) delete[] seq;
#else
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
#endif
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure that the temporary buffer has at least 'need' elements. */
static void ensure_w_capacity(size_t need)
{
    if (need <= w_cap)
        return;
    size_t new_cap = w_cap ? w_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < w_cap; ++i)
        tmp[i] = w_buf[i];
    if (w_buf) delete[] w_buf;
#else
    seq_t *tmp = (seq_t *)realloc(w_buf, new_cap * sizeof(seq_t));
#endif
    w_buf = tmp;
    w_cap = new_cap;
}

/* Check whether the palindrome built by putting 'cand' at both ends
   of the current sequence of length i is square‑free.  `seq` contains
   a(0)…a(i‑1).  The function returns true only if the word is
   square‑free. */
static bool is_squarefree_pal(idx_t i, seq_t cand)
{
    size_t len = 2 * (size_t)i + 1;          /* length of palindrome */
    ensure_w_capacity(len);
    seq_t *w = w_buf;

    /* Build the palindrome */
    w[0] = cand;
    for (idx_t pos = 1; pos <= i; ++pos)
        w[pos] = seq[(size_t)(i - pos)];
    for (idx_t pos = i + 1; pos < (size_t)(2 * i); ++pos)
        w[pos] = seq[(size_t)(pos - i)];
    w[2 * (size_t)i] = cand;

    /* Squares that start at position 0. */
    for (idx_t k = 1; k <= i; ++k) {
        bool equal = true;
        for (idx_t t = 0; t < k; ++t) {
            if (w[(size_t)t] != w[(size_t)(t + k)]) {
                equal = false;
                break;
            }
        }
        if (equal)
            return false;   /* square found */
    }

    /* Squares that end at the last position. */
    for (idx_t k = 1; k <= i; ++k) {
        size_t start = 2 * (size_t)i - 2 * (size_t)k + 1;
        bool equal = true;
        for (idx_t t = 0; t < k; ++t) {
            if (w[start + t] != w[start + t + k]) {
                equal = false;
                break;
            }
        }
        if (equal)
            return false;   /* square found */
    }

    return true;   /* no squares detected */
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    /* Ensure capacity for n+1 terms. */
    ensure_seq_capacity((size_t)n + 1);

    /* Compute missing terms up to n. */
    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        seq_t cand = 0;
        while (!is_squarefree_pal((idx_t)i, cand))
            ++cand;
        seq[i] = cand;
    }

    seq_len = (size_t)n + 1;
    return seq[(size_t)n];
}
