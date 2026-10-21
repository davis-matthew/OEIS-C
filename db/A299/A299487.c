/*
 * A299487  Solution b( ) of the complementary equation a(n) = b(n-1) + b(n-2)
 *          + b(n-3), where a(0) = 1, a(1) = 2, a(2) = 3; see Comments.
 * https://oeis.org/A299487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A299487 – Solution b(n) of the complementary equation
 *      a(n) = b(n-1) + b(n-2) + b(n-3)   (n > 2)
 * Where a(0)=1, a(1)=2, a(2)=3 and
 *      b(0) = least positive integer NOT IN {a(0),a(1),a(2)},
 *      b(n) = least positive integer NOT IN {a(0),...,a(n),b(0),...,b(n-1)}  (n > 0).
 * The function returns b(n) for any n≥0.  Sequence values are stored in seq_t.
 */

#include <stdlib.h>   /* malloc, realloc, free, abort */
#include <stddef.h>   /* size_t */
#include <string.h>   /* memset */

/* static arrays holding the two sequences */
static seq_t *B = NULL;          /* b(n) values                      */
static seq_t *A = NULL;          /* a(n) values                      */
static size_t B_cap = 0, A_cap = 0;

/* bitmap of which positive integers have appeared as a or b values */
static unsigned char *used = NULL;
static size_t used_cap = 0;

/* next integer to try when searching for the next missing number   */
static seq_t next_candidate = 1;

/* ------------------------------------------------------------------ */
/* Dynamically resize the used[] bitmap, guaranteeing at least 'new_cap' entries    */
static void ensure_used_capacity(size_t new_cap) {
    if (used_cap >= new_cap) return;
    size_t cap = used_cap ? used_cap : 4;
    while (cap < new_cap) cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, cap * sizeof(unsigned char));
    if (!tmp) abort();                /* unrecoverable memory failure */
    /* zero any newly allocated part */
    if (used_cap < cap)
        memset(tmp + used_cap, 0, (cap - used_cap) * sizeof(unsigned char));
    used = tmp;
    used_cap = cap;
}

/* ------------------------------------------------------------------ */
/* Find the smallest positive integer not yet marked in 'used'         */
static seq_t find_next_missing(void) {
    seq_t cand = next_candidate;
    for (;;) {
        if ((size_t)cand >= used_cap) ensure_used_capacity((size_t)cand + 1);
        if (!used[(size_t)cand]) break;
        ++cand;
    }
    next_candidate = cand + 1;
    return cand;
}

/* ------------------------------------------------------------------ */
/* Ensure the B array can hold at least 'cap' elements                */
static void ensure_B_capacity(size_t cap) {
    if (B_cap >= cap) return;
    size_t new_cap = B_cap ? B_cap : 4;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    B = tmp;
    B_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure the A array can hold at least 'cap' elements                */
static void ensure_A_capacity(size_t cap) {
    if (A_cap >= cap) return;
    size_t new_cap = A_cap ? A_cap : 4;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    A = tmp;
    A_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* One‑time initialization of all tables.                               */
static void init_sequence(void) {
    if (B) return;            /* already initialised */

    /* initial capacities */
    ensure_B_capacity(4);
    ensure_A_capacity(4);

    /* mark a(0)=1, a(1)=2, a(2)=3 as used; 0 is unused */
    ensure_used_capacity(4);
    used[0] = 0;
    used[1] = used[2] = used[3] = 1;

    /* initialise the first three a values */
    A[0] = 1;  A[1] = 2;  A[2] = 3;

    /* compute b(0), b(1), b(2) sequentially */
    for (size_t i = 0; i < 3; ++i) {
        seq_t val = find_next_missing();
        B[i] = val;
        /* mark the value as used */
        if ((size_t)val >= used_cap) ensure_used_capacity((size_t)val + 1);
        used[(size_t)val] = 1;
    }
}

/* ------------------------------------------------------------------ */
/* Return b(n) for any n ≥ 0.                                          */
seq_t a(idx_t n) {
    init_sequence();           /* lazy initialization */

    /* keep track of the highest index computed so far */
    static idx_t precomputed = 2;   /* indices 0..2 already exist   */

    if (n <= precomputed)
        return B[(size_t)n];

    /* compute missing terms up to the requested index */
    for (idx_t idx = precomputed + 1; idx <= n; ++idx) {
        /* compute a(idx) if idx > 2, and mark as used */
        if (idx > 2) {
            seq_t sum = B[(size_t)idx - 1] + B[(size_t)idx - 2] + B[(size_t)idx - 3];
            if ((size_t)idx >= A_cap) ensure_A_capacity((size_t)idx + 1);
            A[(size_t)idx] = sum;
            if ((size_t)sum >= used_cap) ensure_used_capacity((size_t)sum + 1);
            used[(size_t)sum] = 1;
        }

        /* compute the next b value and mark as used */
        seq_t val = find_next_missing();
        if ((size_t)idx >= B_cap) ensure_B_capacity((size_t)idx + 1);
        B[(size_t)idx] = val;
        if ((size_t)val >= used_cap) ensure_used_capacity((size_t)val + 1);
        used[(size_t)val] = 1;
    }

    precomputed = n;
    return B[(size_t)n];
}
