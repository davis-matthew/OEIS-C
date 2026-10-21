/*
 * A275074  Number of set partitions of [n] such that i-j is a multiple of
 *          seven for all i,j belonging to the same block.
 * https://oeis.org/A275074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=0..6} Bell(floor((n+i)/7))
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 30, u64 n <= 46, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where Bell(k) = number of partitions of a set of size k (A000110) */

#include <stddef.h>  /* for size_t */

/* Maximum required Bell number for n ≤ 10000: floor((10000+6)/7) = 1428.
   Allocate slightly more to be safe. */
#define MAX_BELL 1500

/* Static cache of Bell numbers.  bell[0] = 1, others zero-initialized. */
static seq_t bell[MAX_BELL];
static size_t bell_max = 0;   /* largest index computed so far */
static int bell_initialized = 0;

/* Compute Bell numbers up to index n (inclusive) using the recurrence
   B(n+1) = Σ_{k=0}^{n} C(n, k) * B(k).  The computation is incremental
   and uses only integer arithmetic. */
static void
compute_bell_upto(size_t n)
{
    /* First time: set B(0)=1 and mark initialized. */
    if (!bell_initialized) {
        bell[0] = (seq_t)1;
        bell_initialized = 1;
    }

    if (n <= bell_max)
        return;                 /* already computed */

    /* Compute missing Bell numbers. */
    for (size_t m = bell_max + 1; m <= n; ++m) {
        /* For B(m), use m-1 in the formula:  B(m) = Σ_{k=0}^{m-1} C(m-1, k) * B(k) */
        size_t mm = m - 1;
        seq_t sum = 0;
        seq_t binom = 1;      /* C(mm, 0) */

        for (size_t k = 0; k <= mm; ++k) {
            sum += binom * bell[k];

            if (k != mm) {
                /* Update binom to C(mm, k+1) = C(mm, k) * (mm - k) / (k+1)  */
                binom = (binom * (seq_t)(mm - k)) / (seq_t)(k + 1);
            }
        }
        bell[m] = sum;
    }

    bell_max = n;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    seq_t prod = (seq_t)1;

    for (int i = 0; i < 7; ++i) {
        idx_t r = (n + i) / 7;        /* floor division */

        /* Convert to size_t for array indexing; safe because r <= 1428 for
           n ≤ 10000 and MAX_BELL > 1500. */
        compute_bell_upto((size_t)r);
        prod *= bell[(size_t)r];
    }

    return prod;
}
