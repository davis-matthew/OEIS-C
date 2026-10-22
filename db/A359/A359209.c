/*
 * A359209  Numbers that under iteration of the map x->A359194(x) (binary
 *          complement of 3n) until 0 is reached never exceed the initial
 *          term.
 * https://oeis.org/A359209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A359209.
   a(n) is the n-th integer that, when iterated under the map
   x -> binary complement of (3·x) until 0 is reached, never exceeds its
   initial value.  The sequence is generated lazily in increasing order. */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- global state ---------- */
static seq_t *safe = NULL;          /* sorted list of known safe numbers */
static size_t safe_len = 0;         /* number of elements in 'safe' */
static size_t safe_cap = 0;         /* current capacity of 'safe'   */
static idx_t next_candidate = 1;    /* next integer to test for safety */

/* ---------- helper functions ---------- */

/* Return the number of bits needed to represent x (bit length), 0 for x==0. */
static inline unsigned bitlen_seq_t(seq_t x)
{
    unsigned w = 0;
    while (x) {
        x >>= 1;
        ++w;
    }
    return w;
}

/* Ensure 'safe' can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= safe_cap)
        return;
    size_t new_cap = safe_cap ? safe_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(safe, new_cap * sizeof(seq_t));
    safe = tmp;
    safe_cap = new_cap;
}

/* Binary search: return index of val in 'safe' or -1 if not found. */
static int find_safe_idx(idx_t val)
{
    size_t lo = 0, hi = safe_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)safe[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < safe_len && (idx_t)safe[lo] == val)
        return (int)lo;
    return -1;
}

/* ---------- core algorithm ---------- */

/* Return true if n is a "safe" number (trajectory never exceeds n). */
static bool is_safe(idx_t n)
{
    if (n == 0)
        return true;
    seq_t curr = (seq_t)n;
    const seq_t limit = curr;

    while (curr != 0) {
        seq_t prod = curr * (seq_t)3;
        unsigned w = bitlen_seq_t(prod);
        seq_t mask;
        if (w >= sizeof(seq_t) * 8)
            mask = ~((seq_t)0);
        else
            mask = ((seq_t)1 << w) - (seq_t)1;
        seq_t next = mask - prod;

        if (next > limit)
            return false;
        if (next == 0)
            return true;
        if (find_safe_idx((idx_t)next) >= 0)
            return true;
        curr = next;
    }
    /* should never reach here for positive n */
    return false;
}

/* Return the n-th term of A359209 (a(1)=0). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1, but return 0 */

    /* initialise the safe list with the first term */
    if (safe_len == 0) {
        ensure_capacity(8);
        safe[0] = (seq_t)0;
        safe_len = 1;
        next_candidate = 1;
    }

    /* generate terms until we have at least n of them */
    while (safe_len < n) {
        if (is_safe(next_candidate)) {
            ensure_capacity(safe_len + 1);
            safe[safe_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return safe[n - 1];
}
