/*
 * A364449  Lexicographically earliest sequence where n is banned for n^3
 *          terms after each appearance.
 * https://oeis.org/A364449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements lexicographically earliest sequence where n is banned for n³ terms after each appearance (OEIS A364449). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

/* Global state ---------------------------------------------------------- */
static idx_t computed = 0;          /* highest index computed so far */
static size_t nextAvailCap = 0;     /* current capacity of nextAvail array */
static idx_t *nextAvail = NULL;     /* next position at which value becomes available */

static size_t seqCap = 0;            /* current capacity of sequence array */
static seq_t *seqArr = NULL;        /* array of sequence values, 1‑based */

/* Ensure that nextAvail has room for index `need`. */
static void ensure_next_avail_capacity(idx_t need)
{
    if ((size_t)need < nextAvailCap)
        return;
    size_t newCap = nextAvailCap ? nextAvailCap * 2 : 256;
    while (newCap <= (size_t)need)
        newCap <<= 1;
    idx_t *tmp = (idx_t *)realloc(nextAvail, newCap * sizeof(idx_t));
    if (!tmp) abort();
    for (size_t i = nextAvailCap; i < newCap; ++i)
        tmp[i] = 1;   /* initially available from position 1 */
    nextAvail = tmp;
    nextAvailCap = newCap;
}

/* Ensure that seqArr has room for index `need`. */
static void ensure_seq_capacity(idx_t need)
{
    if ((size_t)need < seqCap)
        return;
    size_t newCap = seqCap ? seqCap * 2 : 256;
    while (newCap <= (size_t)need)
        newCap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seqArr, newCap * sizeof(seq_t));
    if (!tmp) abort();
    seqArr = tmp;
    seqCap = newCap;
}

/* Compute terms up to index `n`.  The routine is idempotent. */
static void compute_up_to(idx_t n)
{
    if (n <= computed)
        return;
    for (idx_t pos = computed + 1; pos <= n; ++pos)
    {
        /* Find the smallest value v that is available at position pos. */
        idx_t v = 1;
        for (;; ++v)
        {
            ensure_next_avail_capacity(v);
            if (nextAvail[v] <= pos)
                break;
        }
        /*! Store the value */
        ensure_seq_capacity(pos);
        seqArr[pos] = (seq_t)v;

        /* Update next availability of v.  It is banned for v³ terms. */
        idx_t cube = v * v * v;
        nextAvail[v] = pos + cube + 1;

        computed = pos;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* defensive fallback for 0, sequence is 1‑based */
    compute_up_to(n);
    return seqArr[n];
}
