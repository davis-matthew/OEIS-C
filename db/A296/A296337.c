/*
 * A296337  a(1) = a(3) = 1, a(2) = 2, a(4) = a(5) = 4; a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)) for n > 5.
 * https://oeis.org/A296337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) with the initial values a(1)=1,
 *            a(2)=2, a(3)=1, a(4)=4, a(5)=4
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Dynamic cache of already-computed terms. The cache is indexed from 1
    and grows as needed. Because seq_t may be an arbitrary‑precision
    integer type the allocation is done with realloc/sizeof(seq_t) and
    the returned void* is explicitly cast to seq_t*.  The standard
    requirement that the code is valid in both C11 and C++17
    dictates this explicit cast.  No constructors are invoked; the
    underlying type supplied by the build is presumed to be a POD or
    otherwise trivially copyable.  This is sufficient for the sequence
    computation because the recurrence only uses addition and
    conversion of indices. */

static seq_t *seq_cache = NULL;   /* cache of computed values, index 0 unused */
static idx_t  seq_cache_len = 0;  /* number of allocated elements (including index 0) */

/* Allocate (or grow) the cache so that it contains terms up to n
   (inclusive).  Values 1..5 are filled explicitly; all higher values
   are computed iteratively using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (n >= seq_cache_len) {
        idx_t new_len = n + 1;                      /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                          /* allocation failure */
        seq_cache = tmp;

        /* Initialise terms that are not already set. */
        for (idx_t i = seq_cache_len; i < new_len; ++i) {
            if (i == 0)
                continue;     /* index 0 is unused */

            if (i <= 5) {      /* fixed initial values */
                switch ((int)i) {
                    case 1: seq_cache[1] = (seq_t)1; break;
                    case 2: seq_cache[2] = (seq_t)2; break;
                    case 3: seq_cache[3] = (seq_t)1; break;
                    case 4: seq_cache[4] = (seq_t)4; break;
                    case 5: seq_cache[5] = (seq_t)4; break;
                }
            } else {          /* recurrence for all i > 5 */
                /* a(i) = a(i - a(i-1)) + a(i - a(i-2)) */
                idx_t idx1 = i - (idx_t)seq_cache[i - 1];
                idx_t idx2 = i - (idx_t)seq_cache[i - 2];
                seq_cache[i] = seq_cache[idx1] + seq_cache[idx2];
            }
        }
        seq_cache_len = new_len;
    }
}

/* Public function: return the nth term of A296337.  The function is
   safe for any order of calls and for any n ≥ 1.  For n = 0 the
   sequence is undefined; here 0 is returned. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;         /* undefined for n = 0 */

    ensure_cache(n);
    return seq_cache[n];
}
