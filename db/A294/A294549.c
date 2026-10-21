/*
 * A294549  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + n + 1, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) + n + 1, a(0)=1, a(1)=2, b(0)=3,
 *            where a() and b() are increasing complementary sequences
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for the generated values */
static seq_t *a_seq = NULL;      /* array of a values     */
static seq_t *b_seq = NULL;      /* array of b values     */
static idx_t  a_cap = 0;         /* capacity of a_seq     */
static idx_t  b_cap = 0;         /* capacity of b_seq     */
static idx_t  a_len = 0;         /* current number of a   */
static idx_t  b_len = 0;         /* current number of b   */
static seq_t  candidate = 0;     /* next integer to test  */

/* Ensure capacity for a_seq (at least needed+1 elements). */
static void ensure_a_cap(idx_t needed)
{
    if (needed >= a_cap) {
        idx_t new_cap = a_cap ? a_cap * 2 : 4;
        while (new_cap <= needed) new_cap <<= 1;
        a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }
}

/* Ensure capacity for b_seq (at least needed+1 elements). */
static void ensure_b_cap(idx_t needed)
{
    if (needed >= b_cap) {
        idx_t new_cap = b_cap ? b_cap * 2 : 4;
        while (new_cap <= needed) new_cap <<= 1;
        b_seq = (seq_t *)realloc(b_seq, new_cap * sizeof(seq_t));
        b_cap = new_cap;
    }
}

/* Binary search: return 1 if target is present in array[0..len-1], else 0. */
static int contains_a(const seq_t *array, idx_t len, seq_t target)
{
    idx_t lo = 0, hi = len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (array[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < len && array[lo] == target);
}

/* Initialise the sequences up to at least n_needed. */
static void init_sequence(idx_t n_needed)
{
    /* first time: allocate minimal capacity and set the initial values */
    if (a_seq == NULL) {
        a_cap = 4;
        a_seq = (seq_t *)calloc(a_cap, sizeof(seq_t));
        a_seq[0] = (seq_t)1;      /* a(0) */
        a_seq[1] = (seq_t)2;      /* a(1) */
        a_len = 2;
    }

    if (b_seq == NULL) {
        b_cap = 4;
        b_seq = (seq_t *)calloc(b_cap, sizeof(seq_t));
        b_seq[0] = (seq_t)3;      /* b(0) */
        b_len = 1;
        candidate = (seq_t)4;     /* next candidate for b */
    }

    /* make sure we have b(1) … b(a_len-1) before generating new a's */
    while (b_len < a_len && b_len <= n_needed) {
        seq_t cand = candidate;
        while (contains_a(a_seq, a_len, cand))
            ++cand;
        ensure_b_cap(b_len);
        b_seq[b_len++] = cand;
        candidate = cand + 1;
    }

    /* generate a(n) and b(n) for n from a_len up to n_needed */
    for (idx_t i = a_len; i <= n_needed; ++i) {
        seq_t a_im1 = a_seq[i - 1];
        seq_t a_im2 = a_seq[i - 2];
        seq_t b_im1 = b_seq[i - 1];
        seq_t n_plus1 = (seq_t)i + (seq_t)1;
        seq_t a_i = a_im1 + a_im2 + b_im1 + n_plus1;

        ensure_a_cap(i);
        a_seq[i] = a_i;
        a_len = i + 1;   /* updated length */

        /* compute b(i) as the next integer not in a_seq */
        seq_t cand = candidate;
        while (contains_a(a_seq, a_len, cand))
            ++cand;
        ensure_b_cap(i);
        b_seq[i] = cand;
        b_len = i + 1;
        candidate = cand + 1;
    }
}

/* required public function */
seq_t a(idx_t n)
{
    if (n >= a_len)
        init_sequence(n);
    return a_seq[n];
}
