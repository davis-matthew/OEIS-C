/*
 * A297291  Solution (a(n)) of the system of 3 complementary equations in
 *          Comments.
 * https://oeis.org/A297291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is part of three complementary sequences a(n) = the least
 *            unused positive integer; b(n) = the least unused integer ≥
 *            a(n)+2; c(n) = a(n) + b(n) – 2. This implementation builds the
 *            sequences incrementally, memoising the a‑values so that calls
 *            may come in any order. The algorithm is O(n) for t...
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static memoisation storage */
static seq_t *A        = NULL;   /* a(n) values */
static size_t A_cap    = 0;      /* allocated size of A */
static idx_t  A_last   = (idx_t)-1;  /* highest index computed */

/* used[i] == 1 if integer i has already appeared as an a, b, or c value */
static unsigned char *used    = NULL;
static size_t  used_cap = 0;

/* next_a and next_b hold the smallest candidate that has not yet been
   rejected during previous iterations */
static idx_t next_a = 1;
static idx_t next_b = 1;

/* ------------------------------------------------------------------ */
/* Ensure that the used array covers indices < 'need'                 */
static void ensure_used_cap(idx_t need)
{
    if (used_cap >= (size_t)need) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap < (size_t)need) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp) abort();                    /* memory exhaustion */
    if (new_cap > used_cap) {
        /* zero the newly allocated elements */
        memset(tmp + used_cap, 0, new_cap - used_cap);
    }
    used      = tmp;
    used_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure that the A array covers indices ≤ 'need'                     */
static void ensure_A_cap(idx_t need)
{
    if (A_cap > (size_t)need) return;
    size_t new_cap = A_cap ? A_cap : 16;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();                    /* memory exhaustion */
    A      = tmp;
    A_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/* Return the n‑th term of the sequence                            */
seq_t a(idx_t n)
{
    /* initialise static tables on first call */
    if (!A) {
        A_cap   = 16;
        A       = (seq_t *)calloc(A_cap, sizeof(seq_t));
        if (!A) abort();

        used_cap = 16;
        used     = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
        if (!used) abort();

        /* a(0) = 1, b(0) = 3, c(0) = 2 */
        A[0] = (seq_t)1;
        ensure_used_cap(4);
        used[1] = 1;   /* a(0) */
        used[3] = 1;   /* b(0) */
        used[2] = 1;   /* c(0) */
        next_a    = 1;
        next_b    = 1;
        A_last    = 0;
    }

    /* compute up to n if necessary */
    if (n <= A_last) return A[n];

    ensure_A_cap(n);
    /* a rough upper bound for used values is 5*n + 10;
       grow more if the sequence values become large */
    ensure_used_cap(n * 5 + 10);

    for (idx_t i = A_last + 1; i <= n; ++i) {
        /* --- find a(i) : least unused integer --- */
        while (true) {
            ensure_used_cap(next_a + 1);
            if (!used[(size_t)next_a]) break;
            ++next_a;
        }
        seq_t a_val = (seq_t)next_a;
        used[(size_t)next_a] = 1;         /* mark a used */

        /* --- find b(i) : least unused integer ≥ a(i)+2 --- */
        idx_t candidate_b = (idx_t)a_val + 2;
        if (candidate_b < next_b) candidate_b = next_b;
        while (true) {
            ensure_used_cap(candidate_b + 1);
            if (!used[(size_t)candidate_b]) break;
            ++candidate_b;
        }
        seq_t b_val = (seq_t)candidate_b;
        used[(size_t)candidate_b] = 1;     /* mark b used */

        /* --- compute c(i) = a(i)+b(i)-2 and mark used --- */
        seq_t c_val = a_val + b_val - 2;
        ensure_used_cap((idx_t)c_val + 1);
        used[(size_t)c_val] = 1;

        /* store a(i) */
        A[i] = a_val;

        /* prepare for next iteration */
        ++next_a;              /* next a is at least a_val+1 */
        next_b = candidate_b;  /* future b's start from this value */
    }

    A_last = n;
    return A[n];
}
