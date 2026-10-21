/*
 * A299545  Solution a( ) of the complementary equation a(n) = b(n-1) +
 *          2*b(n-2) - b(n-3), where a(0) = 1, a(1) = 2, a(2) = 3; see
 *          Comments.
 * https://oeis.org/A299545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n-1) + 2*b(n-2) - b(n-3), a(0) = 1, a(1) = 2, a(2) = 3
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function relies on a mex (minimum excluded number) mechanism to generate a
   complementary sequence b(n).  Both sequences are memoised in static arrays
   that grow only as needed.  The implementation uses only integer arithmetic
   and supports arbitrary‑precision types for seq_t (by following the allowed
   operators only). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *A = NULL;              /* a[n] values          */
static seq_t *B = NULL;              /* b[n] values          */
static uint8_t *used = NULL;         /* bitmap of numbers seen */
static size_t A_cap = 0;             /* capacity of A,B      */
static size_t used_cap = 0;          /* capacity of used     */
static seq_t current_mex = 1;        /* current mex candidate */
static bool initialized = false;     /* have we set up?      */
static idx_t last_computed = 2;      /* highest a[n] known   */

/* ensure bitmap can index value v */
static void ensure_used_capacity(seq_t v)
{
    size_t val = (size_t)v;
    if (val >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 64;
        while (val >= new_cap) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
        if (!tmp) abort();                 /* out of memory */
        /* initialise new area to zero */
        if (new_cap > used_cap)
            memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

/* mark value v as used */
static void mark_used(seq_t v)
{
    ensure_used_capacity(v);
    used[(size_t)v] = 1;
}

/* return current mex, advancing it if necessary */
static seq_t fresh_mex(void)
{
    ensure_used_capacity(current_mex);
    while (used[(size_t)current_mex]) {
        ++current_mex;
        ensure_used_capacity(current_mex);
    }
    return current_mex;
}

/* initialise to first known values and pre‑compute b[0]..b[2] */
static void init_sequences(void)
{
    if (initialized) return;

    /* allocate space for indices 0..3 (will grow as needed) */
    A_cap = 16;
    A = (seq_t *)calloc(A_cap, sizeof(seq_t));
    B = (seq_t *)calloc(A_cap, sizeof(seq_t));
    used_cap = 32;
    used = (uint8_t *)calloc(used_cap, sizeof(uint8_t));

    /* base a values */
    A[0] = (seq_t)1;
    A[1] = (seq_t)2;
    A[2] = (seq_t)3;
    mark_used(A[0]);  mark_used(A[1]);  mark_used(A[2]);

    /* pre‑compute b[0]..b[2] */
    for (idx_t i = 0; i <= 2; ++i) {
        B[i] = fresh_mex();
        mark_used(B[i]);
    }

    /* last a computed is 2 */
    last_computed = 2;
    initialized = true;
}

/* ensure capacity for index up to n */
static void grow_arrays(idx_t up_to)
{
    if ((size_t)up_to + 1 <= A_cap) return;
    size_t new_cap = A_cap;
    while ((size_t)up_to + 1 > new_cap) new_cap <<= 1;
    A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    B = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    A_cap = new_cap;
}

/* OEIS sequence A299545 */
seq_t a(idx_t n)
{
    init_sequences();
    if (n <= last_computed) return A[n];

    grow_arrays(n);
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* compute a[i] for i >= 3 */
        seq_t ai = B[i-1] + (seq_t)2 * B[i-2] - B[i-3];
        A[i] = ai;
        mark_used(ai);

        /* compute b[i] using current mex */
        seq_t bi = fresh_mex();
        B[i] = bi;
        mark_used(bi);
    }
    last_computed = n;
    return A[n];
}
