/*
 * A298111  Solution b( ) of the complementary equation a(n) = a(1)*b(n) -
 *          a(0)*b(n-1) + 2*n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4,
 *          b(2) = 5, and (b(n)) is the increasing sequence of positive
 *          integers not in (a(n)). See Comments.
 * https://oeis.org/A298111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Complementary sequence b(n) is defined by the recurrence
 *   a(n) = 2*b(n) - b(n-1) + 2*n,
 *   a(0)=1, a(1)=2,
 * with initial values b(0)=3, b(1)=4, b(2)=5.
 * The value returned by this function is b(n).
 */

#include <stdlib.h>
#include <string.h>

/* Ensure the dynamic array pointed to by *arr has at least 'needed'
 * elements, reallocating and optionally doubling the capacity. */
static void
ensure_seq_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Simple presence array: present[v] == 1 if v has already appeared
 * in either a or b.  The indices are of type idx_t so values larger
 * than idx_t can never be stored; for the range of n considered here
 * all values fit within idx_t. */
static uint8_t *present = NULL;
static idx_t present_cap = 0;

static void
ensure_present_cap(idx_t v)
{
    if (v >= present_cap) {
        idx_t new_cap = present_cap ? present_cap * 2 : 16;
        while (new_cap <= v) new_cap *= 2;
        uint8_t *tmp = (uint8_t *)realloc(present, new_cap);
        if (!tmp) abort();
        memset(tmp + present_cap, 0, new_cap - present_cap);
        present = tmp;
        present_cap = new_cap;
    }
}

static void
add_present(idx_t v)
{
    ensure_present_cap(v);
    present[v] = 1;
}

static int
is_present(idx_t v)
{
    return v < present_cap && present[v];
}

/* Dynamically allocated sequences.  a_vals holds the a(n) values
 * (needed only for the recurrence), b_vals holds the b(n) values
 * that are requested by the public function. */
static seq_t *a_vals = NULL;
static seq_t *b_vals = NULL;

static size_t a_cap = 0, a_size = 0;
static size_t b_cap = 0, b_size = 0;
static int initialized = 0;

/* Initialise the seed data and set up the presence bitmap. */
static void
init_sequences(void)
{
    if (initialized) return;
    initialized = 1;

    a_cap = 16;
    b_cap = 16;
    a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
    b_vals = (seq_t *)malloc(b_cap * sizeof(seq_t));
    if (!a_vals || !b_vals) abort();

    /* a(0)=1, a(1)=2 */
    ensure_seq_cap(&a_vals, &a_cap, 3);
    a_vals[0] = (seq_t)1;
    a_vals[1] = (seq_t)2;
    a_size = 2;
    add_present(1);
    add_present(2);

    /* b(0)=3, b(1)=4, b(2)=5 */
    b_vals[0] = (seq_t)3;
    b_vals[1] = (seq_t)4;
    b_vals[2] = (seq_t)5;
    b_size = 3;
    add_present(3);
    add_present(4);
    add_present(5);

    /* compute a(2) = 2*b(2) - b(1) + 4 */
    seq_t a2 = (seq_t)2 * b_vals[2] - b_vals[1] + (seq_t)4;
    a_vals[2] = a2;
    a_size = 3;
    add_present((idx_t)a2);
}

/* Public function: return b(n).  Calls are not required to be in
 * increasing order of n – the code maintains static state that can
 * be used in any order. */
seq_t
a(idx_t n)
{
    init_sequences();

    /* Extend sequences until we have generated b[n]. */
    while (b_size <= n) {
        /* find the next integer greater than the last b that does
           not belong to the current output set. */
        seq_t last_b = b_vals[b_size - 1];
        seq_t cand = last_b + (seq_t)1;
        while (is_present((idx_t)cand)) {
            cand += (seq_t)1;
        }

        /* add the new b value */
        ensure_seq_cap(&b_vals, &b_cap, b_size + 1);
        b_vals[b_size] = cand;
        add_present((idx_t)cand);

        /* compute the corresponding a value, if required. */
        idx_t k = b_size;             /* index of the new a */
        if (k >= 2) {
            seq_t prev_b_prev = b_vals[k - 1];
            seq_t a_val = (seq_t)2 * cand - prev_b_prev + (seq_t)2 * k;
            ensure_seq_cap(&a_vals, &a_cap, k + 1);
            a_vals[k] = a_val;
            add_present((idx_t)a_val);
        }
        b_size++;
    }

    return b_vals[n];
}
