/*
 * A348330  Lexicographically earliest sequence of positive integers such that
 *          for any n > 0, the sum of the indices k < n such that a(k) = a(n)
 *          is strictly less than n.
 * https://oeis.org/A348330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A348330
 *
 * Recursive definition: for n > 0, a(n) is the smallest positive integer
 * v such that
 *
 *        Σ{k < n : a(k) = v} k   <   n
 *
 * (the sum of the previous indices where v occurs must be strictly less
 * than the current index).  The sequence is constructed greedily from
 * left to right; this function returns the n‑th term.
 *
 * Implementation notes:
 *   •  sumIdx[i] holds the current sum of indices for value (i+1) (1‑based).
 *   •  The array grows as needed; when a new value is required it is
 *       initialised to 0.
 *   •  Because the routine may be called with n in arbitrary order,
 *       the computed prefix is memoised in a static array.
 *   •  The code follows the C11 standard and compiles as C++17 as well.
 *   •  The use of realloc is cast explicitly for C++ compatibility.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* for NULL, etc. */

/* The types seq_t and idx_t are supplied by the build system. */

static seq_t *seq   = NULL;   /* stored terms, 0‑based */
static size_t seq_cap = 0;    /* capacity of seq */
static size_t seq_len = 0;    /* number of terms stored */

static idx_t *sumIdx = NULL;  /* sum of indices for each value (1‑based) */
static size_t sum_cap = 0;    /* capacity of sumIdx */

/* Make sure sumIdx has room for indices up to v (1‑based). */
static void ensure_sum_cap(size_t v) {
    if (v <= sum_cap) return;          /* already large enough */
    size_t new_cap = sum_cap ? sum_cap : 16;
    while (new_cap < v) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(sumIdx, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);                  /* allocation failure */
    /* initialise the newly allocated part to zero */
    for (size_t i = sum_cap; i < new_cap; ++i)
        tmp[i] = 0;
    sumIdx  = tmp;
    sum_cap = new_cap;
}

/* Make sure seq has room for at least 'needed' elements. */
static void ensure_seq_cap(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq    = tmp;
    seq_cap = new_cap;
}

/* Return the value of the sequence at position n (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1; just a safety guard */

    /* Build the sequence up to n if we haven't yet. */
    while (seq_len < n) {
        idx_t pos = seq_len + 1;    /* current index (1‑based) */

        /* Find the smallest value v whose accumulated index sum is < pos. */
        idx_t v = 1;
        for (;;) {
            ensure_sum_cap((size_t)v);          /* add space for v if needed */
            if (sumIdx[v-1] < pos) break;
            ++v;
        }

        /* Store the chosen value and update its index sum. */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = (seq_t)v;
        sumIdx[v-1] += pos;
        ++seq_len;
    }

    /* n is 1‑based, seq is 0‑based. */
    return seq[n-1];
}
