/*
 * A337932  Number of ways to write n as the sum of two deficient numbers
 *          (A005100).
 * https://oeis.org/A337932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} c(i)*c(n-i), where c(k) = 1 if k is
 *            a * deficient number (sum of proper divisors < k), otherwise 0
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple divisor-sum sieve to identify all
 * deficient numbers up to n.  For each call it recomputes the sieve up to
 * the requested n; with n ≤ 10 000 this stays well below the 2 s limit. */

#include <stdlib.h>
#include <stdint.h>

/* Types supplied by the build system. */
extern seq_t a(idx_t n);

/* Internal storage for the deficiency flags. */
static uint8_t *def_flag = NULL;   /* 1 for deficient, 0 otherwise */
static idx_t   known_n  = 0;       /* largest n for which def_flag is valid */

/* ------------------------------------------------------------------ */
/* Recompute deficiency flags for all numbers up to 'limit'.           */
/* ------------------------------------------------------------------ */
static void recompute_deficiency(idx_t limit)
{
    /* Allocate a fresh array of zeros.  Cast the void* return from calloc
       to the required pointer type – this is valid in both C11 and C++17. */
    uint8_t *new_flag = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    if (!new_flag) exit(EXIT_FAILURE);

    /* Temporary array to accumulate proper-divisor sums.  Use 64‑bit
       integers so we don't overflow when 'limit' is large. */
    uint64_t *sum = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
    if (!sum) {
        free(new_flag);
        exit(EXIT_FAILURE);
    }

    /* Sieve: for each divisor d, add d to all multiples m > d. */
    for (idx_t d = 1; d <= limit / 2; ++d)
        for (idx_t m = 2 * d; m <= limit; m += d)
            sum[m] += (uint64_t)d;

    /* Flag deficient numbers.  1 is always deficient. */
    for (idx_t i = 1; i <= limit; ++i) {
        if (i == 1)
            new_flag[i] = 1;
        else
            new_flag[i] = (sum[i] < (uint64_t)i) ? 1 : 0;
    }

    /* Replace the old table.  The old memory is freed automatically
       by the return of realloc (or here by free). */
    free(def_flag);
    def_flag = new_flag;
    known_n  = limit;
    free(sum);
}

/* ------------------------------------------------------------------ */
/* Ensure that deficiency flags are available up to 'n'.                */
/* ------------------------------------------------------------------ */
static void ensure_deficiency(idx_t n)
{
    if (n <= known_n)
        return;
    recompute_deficiency(n);
}

/* ------------------------------------------------------------------ */
/* Compute a(n).  The index n is ≥ 1.                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* Make sure the deficiency table covers [1..n]. */
    ensure_deficiency(n);

    seq_t cnt = 0;
    idx_t half = n >> 1;          /* floor(n/2) */

    for (idx_t i = 1; i <= half; ++i)
        if (def_flag[i] && def_flag[n - i])
            cnt += (seq_t)1;

    return cnt;
}
