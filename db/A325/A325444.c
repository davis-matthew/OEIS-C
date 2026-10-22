/*
 * A325444  Difference sequence of A325417.
 * https://oeis.org/A325444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A(n+1) - A(n) where A(1) = 1 and for n>1, A(n) is the
 *            smallest positive integer that is not equal to 2*A(k) or
 *            3*A(k)+1 for any k<n, and has not appeared before. This
 *            generates the sequence (1,3,5,7,8,9,11,…). The function below
 *            computes a(n) on demand, caching all previously needed terms...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static state used for memoisation. */
static seq_t *A = NULL;          /* array of A values */
static size_t capA = 0;          /* current capacity of A */
static idx_t  count = 0;         /* number of computed elements in A */

static bool  *used  = NULL;      /* true if a number has appeared in A */
static bool  *banned = NULL;     /* true if a number is forbidden */
static size_t capUsed = 0;       /* capacity of used/banned arrays */

static idx_t next_candidate = 1; /* next integer to test for A(n+1) */

/* Ensure that the arrays for values up to 'v' exist and are initialised to false. */
static void ensure_used_capacity(idx_t v)
{
    if (v < (idx_t)capUsed) return;
    size_t newCap = capUsed ? capUsed * 2 : 16;
    while (newCap <= (size_t)v) newCap <<= 1;
    used   = (bool *)realloc(used,   newCap * sizeof(bool));
    banned = (bool *)realloc(banned, newCap * sizeof(bool));
    memset(used   + capUsed, 0, (newCap - capUsed) * sizeof(bool));
    memset(banned + capUsed, 0, (newCap - capUsed) * sizeof(bool));
    capUsed = newCap;
}

/* Ensure array A has capacity for at least 'n' elements. */
static void ensure_A_capacity(idx_t n)
{
    if (n <= (idx_t)capA) return;
    size_t newCap = capA ? capA * 2 : 16;
    while (newCap < (size_t)n) newCap <<= 1;
    A = (seq_t *)realloc(A, newCap * sizeof(seq_t));
    capA = newCap;
}

/* Compute the next element of A and update the used/banned lists. */
static void add_next(void)
{
    /* Find the next integer that is neither used nor banned. */
    while (true) {
        if (next_candidate >= (idx_t)capUsed) ensure_used_capacity(next_candidate);
        if (!used[(size_t)next_candidate] && !banned[(size_t)next_candidate]) break;
        ++next_candidate;
    }

    idx_t val = next_candidate;
    seq_t A_val = (seq_t)val;

    /* Store new A value. */
    ensure_A_capacity(count + 1);
    A[count] = A_val;
    ++count;

    /* Mark the value as used. */
    used[(size_t)val] = true;

    /* Mark 2*A and 3*A+1 as banned. */
    idx_t b1 = 2 * val;
    idx_t b2 = 3 * val + 1;
    if (b1 >= (idx_t)capUsed) ensure_used_capacity(b1);
    if (b2 >= (idx_t)capUsed) ensure_used_capacity(b2);
    banned[(size_t)b1] = true;
    banned[(size_t)b2] = true;

    /* Advance candidate for next search. */
    ++next_candidate;
}

/* Compute A values until we have at least 'needed' elements (needed >= 1). */
static void compute_up_to(idx_t needed)
{
    while (count < needed) {
        if (count == 0) {
            /* Initialise first element A(1)=1 complying with the OEIS definition. */
            next_candidate = 1;
            add_next(); /* will set A[0]=1 and mark 2,4 as banned */
        } else {
            add_next();
        }
    }
}

/* Main API function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined in OEIS; return 0 as placeholder */
    idx_t needed = n + 1;          /* need A(n+1) to compute difference */
    compute_up_to(needed);

    /* a(n) = A(n+1) - A(n), array indices are zero‑based */
    seq_t diff = A[n] - A[n - 1];
    return diff;
}
