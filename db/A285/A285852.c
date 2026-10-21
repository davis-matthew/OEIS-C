/*
 * A285852  Rectangular array read by rows: T(n,k) is the number of words of
 *          length n on alphabet {0,1,2} that have exactly k records, n>=0,
 *          0<=k<=3.
 * https://oeis.org/A285852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 45, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285852:  T(n,k) = number of words of length n over {0,1,2}
   having exactly k records (0 ≤ k ≤ 3).  The OEIS sequence a(n) is
   obtained by reading the array row‑wise:  a(n) = T(n/4, n%4).  Closed
   forms for the four columns are
        T(n,0) = (n==0) ? 1 : 0
        T(n,1) = 1 + 2^{n‑1} + 3^{n‑1}
        T(n,2) = (3^n – 3)/2
        T(n,3) = (3^{n‑1} – 2^n + 1)/2   */

#include <stdlib.h>
#include <stddef.h>

/*  Static tables for powers of 2 and 3.  The tables are grown as
    needed and are retained across calls. */
static seq_t *pow2 = NULL;
static seq_t *pow3 = NULL;
static idx_t   pow_max = 0;   /* maximum index currently stored */

/*  Ensure that powers up to 'need' are computed.  The arrays are
    allocated (or enlarged) and newly added entries are filled using
    repeated multiplication. */
static void ensure_powers(idx_t need)
{
    if (need <= pow_max)
        return;                      /* already up to date */

    /* Decide new capacity (double until large enough). */
    size_t new_cap = pow_max ? pow_max : 1;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;

    /* Allocate or reallocate arrays. */
    pow2 = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
    pow3 = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
    if (!pow2 || !pow3) {
        exit(1);                     /* memory allocation failed */
    }

    /* Initialize starting element if tables were just created. */
    if (pow_max == 0) {
        pow2[0] = (seq_t)1;
        pow3[0] = (seq_t)1;
        pow_max = 0;
    }

    /* Compute new entries. */
    for (idx_t i = pow_max + 1; i <= need; ++i) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }
    pow_max = need;
}

/*  Main sequence function.  It returns a(n) as defined above.  */
seq_t a(idx_t n)
{
    idx_t row = n / 4u;          /* n = 4*row + col */
    unsigned int col = (unsigned int)(n % 4u);

    /* We need powers up to 'row'.  For column 3 we also use pow2[row]. */
    ensure_powers(row);

    /* Compute the requested entry. */
    switch (col) {
    case 0:
        return (row == 0u) ? (seq_t)1 : (seq_t)0;

    case 1:
        if (row == 0u) return (seq_t)0;
        return (seq_t)1 + pow2[row - 1] + pow3[row - 1];

    case 2:
        if (row == 0u) return (seq_t)0;
        return (pow3[row] - (seq_t)3) / (seq_t)2;

    case 3:
        if (row == 0u) return (seq_t)0;
        return (pow3[row - 1] - pow2[row] + (seq_t)1) / (seq_t)2;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
