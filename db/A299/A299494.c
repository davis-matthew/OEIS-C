/*
 * A299494  Solution a( ) of the complementary equation a(n) = b(n-1) + b(n-2)
 *          + b(n-3), where a(0) = 2, a(1) = 4, a(2) = 6; see Comments.
 * https://oeis.org/A299494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A299494 – complementary equation a(n) = b(n‑1)+b(n‑2)+b(n‑3)
 *
 *  Definition:
 *      a(0) = 2,  a(1) = 4,  a(2) = 6
 *      for n > 2:    a(n) = b(n‑1) + b(n‑2) + b(n‑3)
 *
 *      b(0) = least positive integer not in {a(0), a(1), a(2)}
 *      b(n) = least positive integer not in
 *              {a(0), …, a(n), b(0), …, b(n‑1)}   for n > 0
 *
 *  The function below memoises values in static arrays so that any order
 *  of calls to a(n) produces the correct result for all n ≥ 0.  It
 *  uses a simple bit‑set to keep track of the numbers that have already
 *  appeared in the sequences.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- used‑value bitset ---------- */
static uint64_t *used_bits = NULL;   /* dynamic bit array */
static size_t used_cap_words = 0;    /* number of 64‑bit words allocated */
static idx_t next_candidate = 1;     /* next integer to test for unused */

static void ensure_used_capacity(idx_t val)
{
    size_t needed_words = (size_t)((val >> 6) + 1);
    if (needed_words <= used_cap_words)
        return;

    size_t new_words = used_cap_words ? used_cap_words << 1 : 1;
    while (new_words < needed_words)
        new_words <<= 1;

    uint64_t *new_bits = (uint64_t*)realloc(used_bits,
                   new_words * sizeof(uint64_t));
    if (!new_bits) abort();

    if (used_bits) {
        /* zero the new region */
        memset(new_bits + used_cap_words, 0,
               (new_words - used_cap_words) * sizeof(uint64_t));
    } else {
        memset(new_bits, 0, new_words * sizeof(uint64_t));
    }
    used_bits = new_bits;
    used_cap_words = new_words;
}

static void mark_used(idx_t val)
{
    ensure_used_capacity(val);
    used_bits[(size_t)(val >> 6)] |= (1ULL << (val & 63));
}

static bool is_used(idx_t val)
{
    if ((size_t)(val >> 6) >= used_cap_words)
        return false;
    return (used_bits[(size_t)(val >> 6)] & (1ULL << (val & 63))) != 0;
}

static seq_t next_unused(void)
{
    idx_t cand = next_candidate;
    while (is_used(cand))
        ++cand;
    next_candidate = cand + 1;
    mark_used(cand);
    return (seq_t)cand;
}

/* ---------- storage for a(n) and b(n) ---------- */
static seq_t *a_arr = NULL;
static seq_t *b_arr = NULL;
static size_t a_cap = 0, b_cap = 0;   /* allocated sizes of the arrays */
static idx_t a_len = 0, b_len = 0;    /* number of entries actually stored */
static bool initialized = false;

static void grow_arr(seq_t **arr, size_t *cap, size_t idx)
{
    if ((size_t)idx < *cap)
        return;
    size_t new_cap = *cap ? *cap * 2 : 3;
    while (new_cap <= (size_t)idx)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    if (*arr)
        memset(new_arr + *cap, 0,
               (new_cap - *cap) * sizeof(seq_t));
    *arr = new_arr;
    *cap = new_cap;
}

static void init(void)
{
    /* initialise arrays */
    a_cap = 3;
    b_cap = 3;
    a_arr = (seq_t*)calloc(a_cap, sizeof(seq_t));
    b_arr = (seq_t*)calloc(b_cap, sizeof(seq_t));
    if (!a_arr || !b_arr) abort();

    /* base values of a(n) */
    a_arr[0] = (seq_t)2;
    a_arr[1] = (seq_t)4;
    a_arr[2] = (seq_t)6;
    a_len = 3;
    b_len = 3;

    /* mark used values of a */
    mark_used((idx_t)2);
    mark_used((idx_t)4);
    mark_used((idx_t)6);

    /* compute b(0), b(1), b(2) */
    for (idx_t i = 0; i < 3; ++i) {
        seq_t val = next_unused();   /* automatically marks used */
        b_arr[i] = val;
    }

    initialized = true;
}

static void ensure_upto(idx_t n)
{
    if (!initialized)
        init();
    if (n < a_len)
        return;

    for (idx_t k = a_len; k <= n; ++k) {
        /* a(k) = b(k-1)+b(k-2)+b(k-3)   for k >= 3 */
        seq_t val_a = b_arr[(size_t)(k - 1)] +
                      b_arr[(size_t)(k - 2)] +
                      b_arr[(size_t)(k - 3)];
        grow_arr(&a_arr, &a_cap, (size_t)k);
        a_arr[(size_t)k] = val_a;
        mark_used((idx_t)val_a);

        /* b(k) = least unused positive integer */
        seq_t val_b = next_unused(); /* marks used */
        grow_arr(&b_arr, &b_cap, (size_t)k);
        b_arr[(size_t)k] = val_b;
    }

    a_len = n + 1;
    b_len = n + 1;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < a_len)
        return a_arr[(size_t)n];
    ensure_upto(n);
    return a_arr[(size_t)n];
}
