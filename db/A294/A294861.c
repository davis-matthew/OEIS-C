/*
 * A294861  Solution of the complementary equation a(n) = a(n-2) + b(n-2) + 1,
 *          where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A294861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: complementary increasing sequences a(n) and b(n) are defined by
 * a(0) = 1, a(1) = 2, b(0) = 3, and for n ≥ 2,
 *      a(n) = a(n-2) + b(n-2) + 1
 * with b(n) the least positive integer not yet used by a() or b().
 * The function below returns a(n) for any non‑negative n.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* State of the sequence */
static seq_t *A = NULL; /* a(n) values */
static seq_t *B = NULL; /* b(n) values */
static size_t a_cap = 0;
static size_t b_cap = 0;
static uint8_t *USED = NULL;  /* bitmap of used integers */
static size_t used_cap = 0;
static idx_t next_candidate = 1;   /* smallest unused integer candidate */
static idx_t last_computed = 0;    /* highest index for which a() is known */

/* Ensure capacity of A */
static void ensure_a(size_t min)
{
    if (a_cap >= min) return;
    size_t new_cap = a_cap ? a_cap : 2;
    while (new_cap < min) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    A = tmp;
    a_cap = new_cap;
}

/* Ensure capacity of B */
static void ensure_b(size_t min)
{
    if (b_cap >= min) return;
    size_t new_cap = b_cap ? b_cap : 2;
    while (new_cap < min) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    B = tmp;
    b_cap = new_cap;
}

/* Ensure USED bitmap can address up to index 'min' (inclusive) */
static void ensure_used(size_t min)
{
    if (used_cap > min) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap <= min) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(USED, new_cap * sizeof(uint8_t));
    if (!tmp) abort();
    /* initialise newly added slots */
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
    USED = tmp;
    used_cap = new_cap;
}

/* Mark integer x as used */
static void mark_used(idx_t x)
{
    ensure_used((size_t)x + 1);
    USED[(size_t)x] = 1;
}

/* Find the smallest integer not yet used (starting from current candidate) */
static idx_t find_next_unused(void)
{
    while (1) {
        ensure_used((size_t)next_candidate + 1);
        if (!USED[(size_t)next_candidate]) return next_candidate;
        ++next_candidate;
    }
}

/* One‑time initialisation of the sequence data */
static void init_seq(void)
{
    /* Allocate minimal space */
    ensure_a(2);
    ensure_b(2);

    /* Set initial values */
    A[0] = (seq_t)1;
    A[1] = (seq_t)2;
    B[0] = (seq_t)3;

    /* Mark first three as used */
    mark_used((idx_t)1);
    mark_used((idx_t)2);
    mark_used((idx_t)3);

    /* Start candidate search at 1 */
    next_candidate = 1;

    /* Compute b(1) = least unused integer (which is 4) */
    idx_t b1 = find_next_unused();          /* will return 4 */
    B[1] = (seq_t)b1;
    mark_used(b1);

    last_computed = 1;
}

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (A == NULL) init_seq();

    if (n <= last_computed) {
        return A[n];
    }

    /* Grow the arrays as needed */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Compute a(i) = a(i-2) + b(i-2) + 1 */
        seq_t val = A[(size_t)i - 2] + B[(size_t)i - 2] + (seq_t)1;
        ensure_a((size_t)i + 1);
        A[(size_t)i] = val;
        mark_used((idx_t)val);

        /* Compute the least unused integer for b(i) */
        idx_t next = find_next_unused();
        ensure_b((size_t)i + 1);
        B[(size_t)i] = (seq_t)next;
        mark_used(next);
    }

    last_computed = n;
    return A[(size_t)n];
}
