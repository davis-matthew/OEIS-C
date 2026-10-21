/*
 * A299532  Solution b( ) of the complementary equation a(n) = 2*b(n-1) +
 *          b(n-2), where a(0) = 1, a(1) = 2; see Comments.
 * https://oeis.org/A299532
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

/* The OEIS sequence A299532 (the “b‑sequence” of the complementary
   recurrence).  It is defined by */
 /*   a(0) = 3, a(1) = 4,  a(n) = 2*b(n‑1) + b(n‑2)  for n > 1;   */
 /*   b(0) = least positive integer not in {a(0),a(1)}; */
 /*   b(n) = least positive integer not in {a(0)…a(n), b(0)…b(n‑1)}  for n > 1;   */
 /* The function returns a(n) = b(n).  The algorithm keeps a dynamic
   table of the computed a and b values together with a bitmap of
   numbers that have already appeared in either table.  That bitmap
   allows the “least unused positive integer’’ to be found in O(1)
   amortised time.  */
 
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
 
/* Static state used by the function.  All arrays grow with the
   highest n that has been requested so far. */
static seq_t *A = NULL;          /* a‑sequence  (a(0), a(1), …) */
static seq_t *B = NULL;          /* b‑sequence  (b(0), b(1), …) */
static idx_t   A_cap = 0;        /* current array capacity    */
static idx_t   last_n = 0;       /* largest n that has been
                                   computed (i.e. B[last_n] exists) */

/* Bitmap recording which positive integers have already appeared
   (in either A or B).  The bitmap is indexed by the integer value. */
static uint8_t *used = NULL;
static size_t   used_cap = 0;    /* number of bitmap entries    */
static size_t   min_missing = 1; /* smallest unused positive int */

/* Ensure that the bitmap has at least 'needed'+1 entries. */
static void ensure_used_capacity(size_t needed)
{
    if (needed >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        while (new_cap <= needed) new_cap <<= 1;
        uint8_t *tmp = (uint8_t*)realloc(used, new_cap);
        /* If realloc failed, terminate – the program will abort. */
        if (!tmp) abort();
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

/* Mark that the integer 'val' has now appeared.  'val' is guaranteed
   to be small enough to fit into size_t (the sequence values grow
   roughly linearly and stay well below SIZE_MAX for all n that
   are practically required). */
static void mark_used(seq_t val)
{
    size_t v = (size_t)val;
    ensure_used_capacity(v);
    used[v] = 1;
}

/* Move min_missing up until it points at an unused number. */
static void update_min_missing(void)
{
    while (min_missing <= used_cap && used[min_missing])
        ++min_missing;
}

/* Initialise the tables and compute the first few values. */
static void init_tables(void)
{
    /* Small capacity – it will grow automatically. */
    A_cap = 4;
    A = (seq_t*)malloc(A_cap * sizeof(seq_t));
    B = (seq_t*)malloc(A_cap * sizeof(seq_t));
    if (!A || !B) abort();

    /* Initial values from the definition. */
    A[0] = (seq_t)1;
    A[1] = (seq_t)2;
    B[0] = (seq_t)3;
    B[1] = (seq_t)4;

    /* Initialise bitmap. */
    used_cap = 64;
    used = (uint8_t*)calloc(used_cap, 1);
    if (!used) abort();

    /* Mark the numbers that have already been used. */
    mark_used(A[0]); mark_used(A[1]); mark_used(B[0]); mark_used(B[1]);

    /* Set min_missing correctly. */
    min_missing = 1;
    update_min_missing();

    last_n = 1;   /* we have computed b(0) and b(1) */
}

/* Extend the precomputed tables so that B[n] exists. */
static void extend_to(idx_t n)
{
    if (n <= last_n) return;

    /* Grow the A and B arrays if necessary. */
    if ((idx_t)A_cap <= n) {
        size_t new_cap = A_cap ? A_cap * 2 : 4;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmpA = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        seq_t *tmpB = (seq_t*)realloc(B, new_cap * sizeof(seq_t));
        if (!tmpA || !tmpB) abort();
        A = tmpA; B = tmpB; A_cap = new_cap;
    }

    for (idx_t i = last_n + 1; i <= n; ++i) {
        /* Compute a(i) = 2*b(i-1) + b(i-2). */
        seq_t ai = 2 * B[i-1] + B[i-2];
        A[i] = ai;
        mark_used(ai);

        /* If a(i) occupied the current min_missing, advance it. */
        if ((size_t)ai == min_missing) {
            ++min_missing;
            update_min_missing();
        }

        /* Find the least unused positive integer – that is b(i). */
        update_min_missing();          /* ensure min_missing is unused */
        seq_t bi = (seq_t)min_missing;
        B[i] = bi;
        mark_used(bi);

        /* Advance min_missing past the newly used value. */
        ++min_missing;
        update_min_missing();
    }

    last_n = n;
}

/* The required function. */
seq_t a(idx_t n)
{
    if (!A) init_tables();        /* lazy initialisation */
    extend_to(n);                 /* compute up to n if necessary */
    return B[n];                  /* a(n) is the b‑sequence value */
}
