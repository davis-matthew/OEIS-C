/*
 * A295955  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) + 1, where a(0) = 3, a(1) = 4, b(0) = 1, b(1) = 2, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n) + 1 initial values: a(0)=3,
 *            a(1)=4; b(0)=1, b(1)=2, b(2)=5. The sequences a and b are
 *            increasing complementary sequences. This implementation builds
 *            them on demand, keeping all computed values in static arrays. It
 *            never assumes any order of calls
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* static storage for the sequences and auxiliary indices             */
static seq_t *a_arr = NULL;   /* a(n) values                 */
static seq_t *b_arr = NULL;   /* b(n) values                 */
static idx_t  a_cap = 0;      /* current capacity of a_arr   */
static idx_t  b_cap = 0;      /* current capacity of b_arr   */
static idx_t  computed = 0;   /* highest index that has been computed */

/* indices used while searching for the next unused integer for b(n) */
static idx_t a_scan_idx = 0;  /* current position in a_arr when scanning */
static idx_t b_scan_idx = 0;  /* current position in b_arr when scanning */

/* ------------------------------------------------------------------ */
/* enlarge an array until it can hold at least 'need' elements          */
static void ensure_cap(idx_t need, seq_t **arr, idx_t *cap)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 4;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = tmp;
    *cap = (idx_t)new_cap;
}

/* ------------------------------------------------------------------ */
/* initialise static arrays with the known starting terms            */
static void initialise(void)
{
    if (a_arr) return;                  /* already initialised */

    /* allocate enough space for a[0..1] and b[0..2] */
    ensure_cap(2, &a_arr, &a_cap);
    ensure_cap(3, &b_arr, &b_cap);

    a_arr[0] = (seq_t)3;
    a_arr[1] = (seq_t)4;
    b_arr[0] = (seq_t)1;
    b_arr[1] = (seq_t)2;
    b_arr[2] = (seq_t)5;

    computed = 1;                       /* a(0) and a(1) are ready */
    a_scan_idx = 0;
    b_scan_idx = 0;
}

/* ------------------------------------------------------------------ */
/* find the smallest positive integer not already used in a[0..i-1]   *
 * and b[0..i-1], where i = computed+1 (*i* is the index to be     *
 * computed).  The scanning indices are advanced incrementally.       */
static seq_t next_unused_b(idx_t i)
{
    /* i ≥ 2  ,  current 'computed' == i-1 */
    seq_t candidate = b_arr[i-1] + (seq_t)1;   /* start just after last b */

    while (true) {
        /* advance a_scan_idx while a[a_scan_idx] < candidate */
        while (a_scan_idx <= computed && a_arr[a_scan_idx] < candidate)
            a_scan_idx++;

        /* advance b_scan_idx while b[b_scan_idx] < candidate */
        while (b_scan_idx <= i-1 && b_arr[b_scan_idx] < candidate)
            b_scan_idx++;

        bool used = false;
        if (a_scan_idx <= computed && a_arr[a_scan_idx] == candidate)
            used = true;
        else if (b_scan_idx <= i-1 && b_arr[b_scan_idx] == candidate)
            used = true;

        if (!used) break;   /* candidate is not used */
        candidate++;        /* try the next integer */
    }
    return candidate;
}

/* ------------------------------------------------------------------ */
/* public function required by the build                              */
seq_t a(idx_t n)
{
    initialise();

    /* compute all terms up to index n if they are not already known */
    while (n > computed) {
        idx_t i = computed + 1;            /* index to compute next */

        /* make sure the arrays are large enough */
        ensure_cap(i + 1, &a_arr, &a_cap);
        ensure_cap(i + 1, &b_arr, &b_cap);

        /* determine b(i) */
        seq_t b_val = next_unused_b(i);

        /* store b(i) */
        b_arr[i] = b_val;

        /* compute a(i) using the recurrence */
        a_arr[i] = a_arr[i-1] + a_arr[i-2] + b_val + (seq_t)1;

        /* mark this index as computed */
        computed = i;
    }

    return a_arr[n];
}
