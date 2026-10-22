/*
 * A306346  Start with the sequence S(1) = [0], and for n >= 1 define S(n+1)
 *          to equal the concatenation of S(n) with the RUNS transform of S(n)
 *          when read in reverse order. This sequence is the limit of that
 *          process as n goes to infinity.
 * https://oeis.org/A306346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A306346:
   a(1)=0.
   For n>1, compute the sequence S by starting from [0] and
   repeatedly concatenating S with the RUNS transform of S read
   in reverse order.  a(n) equals S[n‑1] (OEIS offset 1). */

#include <stdlib.h>

static seq_t *S = NULL;      /* sequence storage */
static size_t length = 0;    /* number of elements stored */
static size_t capacity = 0;  /* allocated capacity */

/* Grow the storage to hold at least `needed` elements. */
static void ensure_cap(size_t needed)
{
    if (capacity < needed) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *newS;
#if defined(__cplusplus)
        newS = new seq_t[new_cap];
        for (size_t i = 0; i < length; ++i) newS[i] = S[i];
        delete[] S;
#else
        newS = (seq_t*)realloc(S, new_cap * sizeof(seq_t));
        if (!newS) exit(1);
#endif
        S = newS;
        capacity = new_cap;
    }
}

/* Return the n-th term of the sequence (n ≥ 1).  The function is
   memoised: previously computed terms are kept in a static array
   and reused for later calls. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset, return 0 */

    /* initialise the sequence with the first term if necessary */
    if (!S) {
#if defined(__cplusplus)
        S = new seq_t[16];
#else
        S = (seq_t*)calloc(16, sizeof(seq_t));
#endif
        capacity = 16;
        length = 0;
        S[0] = (seq_t)0;
        length = 1;
    }

    /* expand S until it contains at least n elements */
    while (length < (size_t)n) {
        /* scan the current sequence from the end, computing runs */
        size_t i = length;
        while (i > 0) {
            seq_t val = S[i - 1];
            seq_t cnt = 1;
            i--;
            while (i > 0 && S[i - 1] == val) {
                cnt++;
                i--;
            }
            ensure_cap(length + 1);
            S[length++] = cnt;
        }
    }

    return S[n - 1];
}
