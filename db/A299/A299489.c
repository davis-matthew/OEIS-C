/*
 * A299489  Solution b( ) of the complementary equation a(n) = b(n-1) + b(n-2)
 *          + b(n-3), where a(0) = 1, a(1) = 2, a(2) = 4; see Comments.
 * https://oeis.org/A299489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) returns the complementary sequence b(n) defined by
 *            b(0)=least positive integer not in {a(0),a(1),a(2)}, b(n)=least
 *            positive integer not in {a(0),…,a(n)}∪{b(0),…,b(n‑1)}, with
 *            a(n)=b(n‑1)+b(n‑2)+b(n‑3) (n>2). The returned value is b(n) for
 *            all n≥0
 * Range:     u8 n <= 188, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *a_arr  = NULL;      /* a values up to current index     */
static seq_t *b_arr  = NULL;      /* b values up to current index     */
static uint8_t *used = NULL;      /* bitmap of numbers used so far     */
static size_t  used_cap   = 0;    /* capacity of used bitmap           */
static size_t  arr_cap    = 0;    /* capacity of a_arr and b_arr       */
static idx_t   max_n      = 0;    /* highest computed index            */
static idx_t   next_candidate = 1;/* next candidate for b value        */
static bool    init_done  = false;/* flag indicating initialization done */

/* Ensure the used bitmap can index up to v (inclusive).            */
static void
ensure_used_size(idx_t v)
{
    if (used_cap <= (size_t)v) {
        size_t new_cap = used_cap ? used_cap << 1 : 8;
        if (new_cap <= (size_t)v) new_cap = (size_t)v + 1;
        uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
        if (!tmp) exit(EXIT_FAILURE);          /* out of memory */
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used     = tmp;
        used_cap = new_cap;
    }
}

/* Ensure the a/b arrays can hold at least need+1 elements.            */
static void
ensure_arr_size(idx_t need)
{
    if (arr_cap <= (size_t)need) {
        size_t new_cap = arr_cap ? arr_cap << 1 : 8;
        if (new_cap <= (size_t)need) new_cap = (size_t)need + 1;
        seq_t *tmp_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        seq_t *tmp_b = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        if (!tmp_a || !tmp_b) exit(EXIT_FAILURE);               /* out of memory */
        a_arr = tmp_a;
        b_arr = tmp_b;
        arr_cap = new_cap;
    }
}

/* Perform one-time initialization for indices 0..2.                  */
static void
initialize(void)
{
    /* base a(0)=1, a(1)=2, a(2)=4                                        */
    ensure_arr_size(2);
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)2;
    a_arr[2] = (seq_t)4;

    /* mark these numbers as used                                         */
    ensure_used_size(4);
    used[1] = 1;
    used[2] = 1;
    used[4] = 1;

    /* compute b(0), b(1), b(2): smallest unused integers               */
    for (idx_t i = 0; i <= 2; ++i) {
        while (true) {
            if ((idx_t)next_candidate >= used_cap)
                ensure_used_size((idx_t)next_candidate);
            if (!used[(idx_t)next_candidate]) break;
            ++next_candidate;
        }
        b_arr[i] = (seq_t)next_candidate;
        used[next_candidate] = 1;
        ++next_candidate;
    }

    max_n     = 2;
    init_done = true;
}

/* Compute values up to index n (inclusive).                          */
static void
compute_up_to(idx_t n)
{
    if (!init_done) initialize();

    if (n <= max_n) return;               /* already computed */

    ensure_arr_size(n);

    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* a(i) = b(i-1) + b(i-2) + b(i-3)                               */
        seq_t sum = b_arr[i - 1] + b_arr[i - 2] + b_arr[i - 3];
        a_arr[i] = sum;

        idx_t idx_sum = (idx_t)sum;
        ensure_used_size(idx_sum);
        used[idx_sum] = 1;

        /* find next unused integer for b(i)                               */
        while (true) {
            if ((idx_t)next_candidate >= used_cap)
                ensure_used_size((idx_t)next_candidate);
            if (!used[(idx_t)next_candidate]) break;
            ++next_candidate;
        }
        b_arr[i] = (seq_t)next_candidate;
        used[next_candidate] = 1;
        ++next_candidate;
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return b_arr[n];
}
