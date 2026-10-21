/*
 * A290380  Analog of Motzkin sums for Coxeter type D.
 * https://oeis.org/A290380
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2) * A001006(n-2) (Motzkin numbers)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the build.  This function is
 * not static, so it has external linkage as required. */

#include <stddef.h>   /* size_t */

/* Maximum n expected (≈10000).  The array is large enough
 * for all required indices. */
#define MAX_INDEX 10005

/* Static array to store computed Motzkin numbers. */
static seq_t motzkin[MAX_INDEX];
/* Number of indices that have been initialized: next index to compute. */
static idx_t motzkin_len = 0;

/* Ensure Motzkin numbers are pre‑computed up to index 'up_to' inclusive. */
static void ensure_motzkin(idx_t up_to)
{
    if (motzkin_len == 0) {
        /* Initial values: M(0)=1, M(1)=1. */
        motzkin[0] = (seq_t)1;
        motzkin[1] = (seq_t)1;
        motzkin_len = 2;       /* next index to compute is 2 */
    }
    if (motzkin_len > up_to)
        return;                /* already sufficient */

    /* Compute successive Motzkin numbers using the recurrence: */
    /*   M(n) = ((2n+1) M(n-1) + (3n-3) M(n-2)) / (n+2) */
    for (idx_t n = motzkin_len; n <= up_to; ++n) {
        if (n == 0 || n == 1) {
            /* already set */
            continue;
        }
        seq_t term1 = (seq_t)(2*n + 1) * motzkin[n - 1];
        seq_t term2 = (seq_t)(3*n - 3) * motzkin[n - 2];
        seq_t sum   = term1 + term2;
        motzkin[n]  = sum / (n + 2);   /* division is exact */
    }
    motzkin_len = up_to + 1; /* all indices 0..up_to ready */
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* undefined below offset, safe value */

    idx_t idx = n - 2;                  /* Motzkin index needed */
    ensure_motzkin(idx);
    return (seq_t)(n - 2) * motzkin[idx];
}
