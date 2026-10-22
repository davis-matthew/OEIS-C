/*
 * A305847  Solution a() of the complementary equation a(n) + b(n) = 5*n,
 *          where a(1) = 1. See Comments.
 * https://oeis.org/A305847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) + b(n) = 5*n with a(1)=1, a and b increasing and
 *            complementary (each positive integer appears exactly once in a
 *            or b). The function below computes a(n) by a simple incremental
 *            search. It uses O(n) time and memory. No floating point or
 *            external utilities are needed
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system. */
static seq_t *a_arr   = NULL;   /* storage for a(n) values (0‑based: a_arr[0] = a(1)) */
static size_t   a_cap = 0;      /* capacity of a_arr                                */
static unsigned char *used_arr = NULL; /* bitmap of numbers already used by a or b */
static size_t   used_cap = 0;    /* capacity of used_arr                            */
static idx_t   current_n = 0;    /* number of a-values that have been computed      */

/* Ensure a_arr can hold at least n elements.  Preserve existing data. */
static void ensure_a_capacity(idx_t n)
{
    size_t need = (size_t)n;
    if (a_cap >= need)
        return;
    size_t new_cap = a_cap ? a_cap * 2 : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(EXIT_FAILURE);
    a_arr = new_arr;
    a_cap = new_cap;
}

/* Ensure used_arr can hold indices up to 5*n+4 (inclusive).  Zero
   any new entries. */
static void ensure_used_capacity(idx_t n)
{
    size_t need = (size_t)5 * (size_t)n + 5;
    if (used_cap >= need)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 1;
    while (new_cap < need)
        new_cap <<= 1;
    unsigned char *new_arr = (unsigned char *)realloc(used_arr, new_cap * sizeof(unsigned char));
    if (!new_arr)
        exit(EXIT_FAILURE);
    if (new_cap > used_cap)   /* new space must be cleared */
        memset(new_arr + used_cap, 0, (new_cap - used_cap) * sizeof(unsigned char));
    used_arr = new_arr;
    used_cap = new_cap;
}

/* Initialisation for n = 1.  a(1)=1, b(1)=4. */
static void initialise(void)
{
    if (current_n > 0)
        return;
    ensure_a_capacity(1);
    ensure_used_capacity(1);
    a_arr[0] = (seq_t)1;
    used_arr[1] = 1;          /* a(1) used      */
    used_arr[4] = 1;          /* b(1) = 5-1 =4  */
    current_n = 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset is 1; n=0 is unused  */
    initialise();

    if (n <= current_n)
        return a_arr[n - 1];

    /* Grow arrays to accommodate up to n and 5*n numbers. */
    ensure_a_capacity(n);
    ensure_used_capacity(n);

    /* Compute missing a-values up to n. */
    while (current_n < n) {
        ++current_n;                /* we are computing a(current_n) */
        seq_t prev_a   = a_arr[current_n - 2];
        seq_t total    = (seq_t)5 * (seq_t)current_n;
        seq_t cand     = prev_a + 1;

        while (true) {
            size_t idx_c = (size_t)cand;
            seq_t b_val   = total - cand;
            size_t idx_b = (size_t)b_val;

            if (idx_c < used_cap && idx_b < used_cap &&
                !used_arr[idx_c] && !used_arr[idx_b]) {
                a_arr[current_n - 1] = cand;
                used_arr[idx_c] = 1;
                used_arr[idx_b] = 1;
                break;
            }
            ++cand;
        }
    }

    return a_arr[n - 1];
}
