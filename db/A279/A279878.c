/*
 * A279878  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 213", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A279878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A279878: Binary representation of the x‑axis of Rule 213 cellular automaton.
   The value a(n) can be written as a repunit followed by zeros:
   - If n is odd, a(n) = 111…1₁ ( (n+1)/2 ones ).
   - If n is even (n≥4), a(n) = 111…1₂ ( (n/2+1) ones ) × 10^{n/2}.
   a(0)=1, a(1)=1, a(2)=10.  The function below uses these formulas,
   precomputing repunits and powers of ten on demand.  It works for
   uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128, or an
   arbitrary‑precision integer class.  No temporary overflow is
   detected – it is acceptable as values beyond the type’s range are
   not tested.  */
#include <stdlib.h>   /* for malloc, realloc */

/* Forward declarations of helper routines used for dynamic arrays. */
static void ensure_repunit(size_t needed);
static void ensure_pow10(size_t needed);

/* static storage for the cached repunits and 10^k values */
static seq_t *repunit_arr   = NULL;
static size_t repunit_len   = 0;   /* number of elements currently valid */
static size_t repunit_cap   = 0;

static seq_t *pow10_arr     = NULL;
static size_t pow10_len     = 0;   /* number of elements currently valid */
static size_t pow10_cap     = 0;

/* Grow the repunit array to hold indices up to 'needed' (inclusive).
   Indices are 1‑based: repunit_arr[1] = 1, repunit_arr[2] = 11, ...   */
static void ensure_repunit(size_t needed)
{
    if (repunit_len <= needed) {
        if (repunit_cap < needed + 1) {
            size_t new_cap = (repunit_cap == 0) ? (needed * 2 + 4) : (repunit_cap * 2);
            repunit_arr = (seq_t *)realloc(repunit_arr, new_cap * sizeof(seq_t));
            repunit_cap = new_cap;
        }
        /* initialize base if array is empty */
        if (repunit_len == 0) {
            repunit_arr[1] = (seq_t)1;
            repunit_len   = 2; /* indices 0 unused, 1 filled */
        }
        /* fill in missing repunits */
        while (repunit_len <= needed) {
            repunit_arr[repunit_len] =
                repunit_arr[repunit_len - 1] * (seq_t)10 + (seq_t)1;
            repunit_len++;
        }
    }
}

/* Grow the 10^k array to hold 10^k for all k≤needed.  Index 0 holds 1. */
static void ensure_pow10(size_t needed)
{
    if (pow10_len <= needed) {
        if (pow10_cap < needed + 1) {
            size_t new_cap = (pow10_cap == 0) ? (needed * 2 + 4) : (pow10_cap * 2);
            pow10_arr = (seq_t *)realloc(pow10_arr, new_cap * sizeof(seq_t));
            pow10_cap = new_cap;
        }
        /* initialize base if array is empty */
        if (pow10_len == 0) {
            pow10_arr[0] = (seq_t)1;
            pow10_len    = 1;
        }
        /* fill in missing powers of ten */
        while (pow10_len <= needed) {
            pow10_arr[pow10_len] = pow10_arr[pow10_len - 1] * (seq_t)10;
            pow10_len++;
        }
    }
}

seq_t a(idx_t n)
{
    /* treat small indices directly */
    if (n == 0u || n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)10;

    /* determine how many repunits and powers of ten we need */
    if (n & 1u) {     /* odd n */
        size_t reps = (size_t)((n + 1u) / 2u);   /* number of ones */
        /* ensure repunit[reps] exists */
        ensure_repunit(reps);
        return repunit_arr[reps];
    } else {          /* even n */
        size_t t = (size_t)(n / 2u);          /* number of zeros to append */
        size_t reps = t + 1u;                 /* number of ones */
        ensure_repunit(reps);
        ensure_pow10(t);
        return repunit_arr[reps] * pow10_arr[t];
    }
}
