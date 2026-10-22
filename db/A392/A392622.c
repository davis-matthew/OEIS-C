/*
 * A392622  Lexicographically least increasing sequence of positive integers
 *          so that no term is the sum of two consecutive terms, or except for
 *          the first term a partial sum of the sequence.
 * https://oeis.org/A392622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Implements OEIS A392622.
   The sequence is defined greedily:

   a(1) = 1, a(2) = 2.
   For n > 2, a(n) is the smallest integer larger than a(n‑1) that is
   (i) not equal to the sum of any two consecutive terms already present,
   (ii) not equal to any partial sum of the sequence except the first term
   (which is 1).

   Internally the function keeps the sequence, the running prefix
   sums and two simple dynamic arrays that store all forbidden
   two‑term sums and all forbidden prefix sums.  Membership checks are
   performed by a linear search – the arrays grow only as needed
   and remain small enough for the required n ≤ 10 000.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- dynamic integer array ---------- */
typedef struct {
    seq_t *data;
    size_t len;
    size_t cap;
} intarray;

static void intarray_init(intarray *a, size_t init_cap)
{
    a->len = 0;
    a->cap = init_cap;
    a->data = (seq_t*)malloc(init_cap * sizeof(seq_t));
    if (!a->data) exit(1);
}

static void intarray_push(intarray *a, seq_t v)
{
    if (a->len == a->cap) {
        size_t new_cap = a->cap ? a->cap << 1 : 4;
        seq_t *tmp = (seq_t*)realloc(a->data, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        a->data = tmp;
        a->cap = new_cap;
    }
    a->data[a->len++] = v;
}

static bool intarray_contains(const intarray *a, seq_t v)
{
    for (size_t i = 0; i < a->len; ++i)
        if (a->data[i] == v) return true;
    return false;
}

/* ---------- global memoised state ---------- */
static bool            initialized = false;
static seq_t           *seq          = NULL;
static seq_t           *pref         = NULL;
static size_t          seq_len       = 0;
static size_t          seq_cap       = 0;
static intarray        forbidden_sum2;
static intarray        forbidden_pref;

static void ensure_capacity(size_t needed)
{
    if (seq_len + 1 >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 4;
        /* grow seq array */
        seq_t *tmp_seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp_seq) exit(1);
        seq = tmp_seq;
        /* grow pref array (size = new_cap + 1) */
        seq_t *tmp_pref = (seq_t*)realloc(pref, (new_cap + 1) * sizeof(seq_t));
        if (!tmp_pref) exit(1);
        pref = tmp_pref;
        seq_cap = new_cap;
    }
    /* ensure arrays for forbidden values are large enough             */
    if (forbidden_sum2.cap < needed) {
        size_t new_cap = forbidden_sum2.cap ? forbidden_sum2.cap << 1 : 4;
        seq_t *tmp = (seq_t*)realloc(forbidden_sum2.data,
                                     new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        forbidden_sum2.data = tmp;
        forbidden_sum2.cap = new_cap;
    }
    if (forbidden_pref.cap < needed) {
        size_t new_cap = forbidden_pref.cap ? forbidden_pref.cap << 1 : 4;
        seq_t *tmp = (seq_t*)realloc(forbidden_pref.data,
                                     new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        forbidden_pref.data = tmp;
        forbidden_pref.cap = new_cap;
    }
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, but never called */

    if (!initialized) {
        initialized = true;
        seq_cap = 4;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        pref = (seq_t*)malloc((seq_cap + 1) * sizeof(seq_t));
        seq[0] = 0; /* unused placeholder */
        pref[0] = 0;
        intarray_init(&forbidden_sum2, 4);
        intarray_init(&forbidden_pref, 4);
    }

    while (seq_len < (size_t)n) {
        seq_t last  = (seq_len > 0) ? seq[seq_len - 1] : 0;
        seq_t cand  = last + 1;
        /* find the smallest admissible candidate */
        for (;;) {
            if (!intarray_contains(&forbidden_sum2, cand) &&
                !intarray_contains(&forbidden_pref, cand))
                break;
            ++cand;
        }

        /* append to sequence */
        ensure_capacity(seq_len + 1);
        seq[seq_len] = cand;
        /* update prefix sum array */
        pref[seq_len + 1] = pref[seq_len] + cand;
        seq_len++;

        /* record the new forbidden states */
        if (seq_len >= 2) {
            seq_t new_sum2 = seq[seq_len - 2] + cand; /* last two terms */
            intarray_push(&forbidden_sum2, new_sum2);
        }
        if (seq_len > 1) {                      /* pref[seq_len] > 1   */
            intarray_push(&forbidden_pref, pref[seq_len]);
        }
    }

    return seq[n - 1];
}
