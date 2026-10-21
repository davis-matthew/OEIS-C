/*
 * A298110  Solution (b(n)) of the near-complementary equation a(n) =
 *          a(1)*b(n) - a(0)*b(n-1) + n, where a(0) = 1, a(1) = 2, b(0) = 3,
 *          b(1) = 4, b(2) = 5, and (b(n)) is the increasing sequence of
 *          positive integers not in (a(n)). See Comments.
 * https://oeis.org/A298110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A298110: the increasing set of positive integers not appearing in a
   near‑complementary sequence a(n) that satisfies a(n) = 2*b(n) – b(n-1) + n
   with a(0)=1, a(1)=2, b(0)=3,b(1)=4,b(2)=5.  This implementation
   builds the complementary sequences iteratively, storing all already
   used integers so that each new b(n) is the smallest unused positive
   integer.  The algorithm is linear in the required index. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *A = NULL;   /* a‑sequence values (only for recurrence) */
static seq_t *B = NULL;   /* b‑sequence values that are requested */

static size_t a_cap = 0;  /* allocated capacity of A and B */
static size_t cur_sz = 0; /* number of precomputed indices (next to compute) */

/* bitmap of integers that have appeared in either sequence */
static bool *used = NULL;
static size_t used_sz = 0;
static seq_t candidate = 1;   /* next integer to test for b */

/* enlarge the used array to cover values up to 'new_max' (inclusive) */
static void extend_used(size_t new_max) {
    if (new_max < used_sz) return;
    size_t new_size = used_sz ? used_sz : 64;
    while (new_size <= new_max) new_size <<= 1;
    bool *tmp = (bool *)realloc(used, new_size * sizeof(bool));
    if (!tmp) abort();
    for (size_t i = used_sz; i < new_size; ++i) tmp[i] = false;
    used = tmp;
    used_sz = new_size;
}

/* initialise with the first three values of each sequence */
static void init_arrays(void) {
    /* initial capacity for A and B */
    a_cap = 16;
    A = (seq_t *)malloc(a_cap * sizeof(seq_t));
    B = (seq_t *)malloc(a_cap * sizeof(seq_t));
    if (!A || !B) abort();

    /* base values as given in the definition */
    A[0] = (seq_t)1;  B[0] = (seq_t)3;
    A[1] = (seq_t)2;  B[1] = (seq_t)4;
    B[2] = (seq_t)5;  /* b(2) is fixed, a(2) will be computed below */

    cur_sz = 3;   /* indices 0,1,2 have been set for b; a(0),a(1) are set */

    /* mark used numbers 1,2,3,4,5 */
    extend_used(8);
    used[1] = used[2] = used[3] = used[4] = used[5] = true;

    /* compute a(2) = 2*b(2) – b(1) + 2 */
    seq_t a2 = (seq_t)2 * B[2] - B[1] + (seq_t)2;
    A[2] = a2;
    if ((size_t)a2 >= used_sz) extend_used((size_t)a2);
    used[(size_t)a2] = true;
}

/* ensure that indices up to n have been computed */
static void ensure_upto(idx_t n) {
    if (!A) init_arrays();          /* lazily initialise on first call */

    if ((idx_t)cur_sz > n) return;  /* already have enough values */

    idx_t start = cur_sz;           /* first index to compute */

    for (idx_t idx = start; idx <= n; ++idx) {
        /* grow sequences storage if necessary */
        if ((size_t)idx >= a_cap) {
            a_cap <<= 1;
            A = (seq_t *)realloc(A, a_cap * sizeof(seq_t));
            B = (seq_t *)realloc(B, a_cap * sizeof(seq_t));
            if (!A || !B) abort();
        }

        /* find the next unused positive integer for b[idx] */
        seq_t val = candidate;
        while (true) {
            if ((size_t)val >= used_sz) extend_used((size_t)val);
            if (!used[(size_t)val]) break;
            ++val;                     /* continue searching */
        }
        B[idx] = val;
        used[(size_t)val] = true;
        candidate = val + (seq_t)1;

        /* compute a[idx] = 2*b[idx] – b[idx-1] + idx (recurrence) */
        if (idx >= 2) {
            seq_t a_val = (seq_t)2 * B[idx] - B[idx-1] + (seq_t)idx;
            A[idx] = a_val;
            if ((size_t)a_val >= used_sz) extend_used((size_t)a_val);
            used[(size_t)a_val] = true;
        }
    }
    cur_sz = (size_t)n + 1;          /* new number of precomputed indices */
}

seq_t a(idx_t n) {
    ensure_upto(n);
    return B[n];
}
