/*
 * A318040  Number of nX3 0..1 arrays with every element unequal to 0, 2, 3 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (order 21) for A318040: */
/* a(n) =  7*a(n-1) -15*a(n-2) +21*a(n-3) -25*a(n-4) + 6*a(n-5)
   + 9*a(n-6) -22*a(n-7) +44*a(n-8) -16*a(n-9) +32*a(n-10)
  -20*a(n-11) -31*a(n-12) -a(n-13)  +2*a(n-14) -2*a(n-15)
  -18*a(n-16) +3*a(n-17) +12*a(n-18) +14*a(n-19) +3*a(n-20)
  -2*a(n-21)          for n > 23
  with initial terms a(1)…a(23) given below. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* First 23 terms (exact values fit in 64 bits).  For smaller
       seq_t types these are automatically truncated when cast. */
    static const seq_t init[23] = {
        (seq_t)1ULL,                /* a(1) */
        (seq_t)2ULL,                /* a(2) */
        (seq_t)8ULL,                /* a(3) */
        (seq_t)21ULL,               /* a(4) */
        (seq_t)86ULL,               /* a(5) */
        (seq_t)377ULL,              /* a(6) */
        (seq_t)1648ULL,             /* a(7) */
        (seq_t)7261ULL,             /* a(8) */
        (seq_t)31969ULL,            /* a(9) */
        (seq_t)140758ULL,           /* a(10) */
        (seq_t)619822ULL,           /* a(11) */
        (seq_t)2729394ULL,          /* a(12) */
        (seq_t)12018832ULL,         /* a(13) */
        (seq_t)52924798ULL,         /* a(14) */
        (seq_t)233054021ULL,        /* a(15) */
        (seq_t)1026252071ULL,       /* a(16) */
        (seq_t)4519095292ULL,       /* a(17) */
        (seq_t)19899811452ULL,      /* a(18) */
        (seq_t)87628711701ULL,      /* a(19) */
        (seq_t)385872557006ULL,     /* a(20) */
        (seq_t)1699187715083ULL,    /* a(21) */
        (seq_t)7482363905416ULL,    /* a(22) */
        (seq_t)32948548958936ULL    /* a(23) */
    };

    /* For n <= 23, return the stored value. */
    if (n <= 23U) {
        return init[(size_t)(n - 1U)];
    }

    /* Allocate an array to hold terms up to n.  The array is 1‑based
       for convenience: arr[1] = a(1). */
    idx_t size = n + 1;                  /* includes index 0 unused */
    seq_t *arr = (seq_t *)malloc(size * sizeof(seq_t));
    if (!arr) return (seq_t)0;           /* allocation failed → zero */

    /* Initialise with the known first 23 terms. */
    for (idx_t i = 1; i <= 23U; ++i) {
        arr[i] = init[(size_t)(i - 1U)];
    }

    /* Coefficients for the recurrence; index 0 → a(n-1), index 20 → a(n-21). */
    static const int coeff[21] = {
        7,     -15,    21,     -25,     6,
        9,     -22,    44,     -16,    32,
       -20,    -31,    -1,     2,     -2,
       -18,     3,    12,     14,     3,   -2
    };

    /* Compute terms from 24 up to n using the recurrence. */
    for (idx_t i = 24U; i <= n; ++i) {
        seq_t sum = 0;
        for (size_t k = 0; k < 21U; ++k) {
            /* arr[i - (k+1)] is a(i-(k+1)) */
            sum += (seq_t)coeff[k] * arr[i - (k + 1)];
        }
        arr[i] = sum;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
