/*
 * A299531  Solution a( ) of the complementary equation a(n) = 2*b(n-1) +
 *          b(n-2), where a(0) = 1, a(1) = 2; see Comments.
 * https://oeis.org/A299531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the recurrence for OEIS sequence A299531:
   a(0) = 1, a(1) = 2,
   a(n) = 2*b(n-1) + b(n-2) for n > 1
   where b is the least positive integer not yet appearing in
   the union of the a- and b-sequences up to that point.  The
   function is memoised and works for any order of calls. */

#include <stdlib.h>
#include <string.h>

static seq_t *a_arr = NULL;     /* values of a[n] */
static seq_t *b_arr = NULL;     /* values of b[n] */
static size_t a_cap = 0;        /* current capacity of a_arr */
static size_t b_cap = 0;        /* current capacity of b_arr */

static unsigned char *used = NULL;   /* bitset of used numbers */
static size_t used_cap = 0;          /* capacity of used array */
static idx_t next_candidate = 1;     /* smallest unused positive integer */

static int initialized = 0;

/* Ensure that 'used' array can hold index 'v'. */
static void
ensure_used_capacity(size_t v)
{
    if (used == NULL) {
        used_cap = 128;
        used = (unsigned char *)calloc(used_cap, 1);
        if (used == NULL) exit(EXIT_FAILURE);
    }
    while (v >= used_cap) {
        size_t old = used_cap;
        used_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used, used_cap);
        if (tmp == NULL) exit(EXIT_FAILURE);
        used = tmp;
        memset(used + old, 0, used_cap - old);
    }
}

/* Mark integer 'idx' as used and advance the candidate if necessary. */
static void
mark_used(size_t idx)
{
    ensure_used_capacity(idx);
    if (!used[idx]) {
        used[idx] = 1;
        if (idx == (size_t)next_candidate) {
            do {
                ++next_candidate;
                ensure_used_capacity((size_t)next_candidate);
            } while (used[(size_t)next_candidate]);
        }
    }
}

/* Grow any of the a/b arrays to accommodate 'len' elements. */
static void
grow_array(seq_t **arr, size_t *cap, size_t len)
{
    if (len >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : *cap;
        while (len >= new_cap) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (*arr == NULL) exit(EXIT_FAILURE);
        *cap = new_cap;
    }
}

/* Initialisation: set a(0), a(1), b(0), b(1) and mark them as used. */
static void
init_sequence(void)
{
    /* allocate initial storage */
    grow_array(&a_arr, &a_cap, 4);
    grow_array(&b_arr, &b_cap, 4);

    a_arr[0] = 1;
    a_arr[1] = 2;
    mark_used(1);
    mark_used(2);

    /* b(0) */
    seq_t b0 = (seq_t)next_candidate;   /* should be 3 */
    mark_used((size_t)b0);
    b_arr[0] = b0;

    /* b(1) */
    seq_t b1 = (seq_t)next_candidate;   /* should be 4 */
    mark_used((size_t)b1);
    b_arr[1] = b1;

    initialized = 1;
}

/* Compute the sequence up to index 'n' (inclusive). */
static void
ensure_computed_upto(idx_t n)
{
    if (!initialized) init_sequence();

    /* current last computed a index */
    static idx_t last = 1;
    if (n <= last) return;

    /* Ensure arrays are large enough */
    grow_array(&a_arr, &a_cap, (size_t)n + 1);
    grow_array(&b_arr, &b_cap, (size_t)n + 1);

    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = 2*b(i-1) + b(i-2) */
        seq_t ai = 2 * b_arr[i - 1] + b_arr[i - 2];
        a_arr[i] = ai;
        mark_used((size_t)ai);

        /* b(i) = least unused positive integer */
        seq_t bi = (seq_t)next_candidate;  /* current candidate unused */
        mark_used((size_t)bi);
        b_arr[i] = bi;
    }
    last = n;
}

seq_t a(idx_t n)
{
    ensure_computed_upto(n);
    return a_arr[n];
}
