/*
 * A293527  E.g.f.: exp(x/Product_{k>0} (1 - x^k)).
 * https://oeis.org/A293527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>0 a(n) = (n-1)! * Σ_{k=1..n} k * P(k-1) * a(n-k)
 *            / (n-k)! , where P(k) is the partition number (OEIS A000041)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a direct recurrence that avoids
   explicit factorial divisions that produce fractions.  For each n
   we compute

        a(n) = Σ_{k=1..n} k * P(k-1) * a(n-k) * R(n,k)

   with R(n,k) = ∏_{t=n-k+1}^{n-1} t .
   R can be produced iteratively: R(⋯,1)=1 and for k>1
   R(⋯,k) = R(⋯,k-1) * (n-k+1). */

#include <stdlib.h>

/* Static storage for computed values.  All arrays are zero‑initialised
   when first allocated.  The size is doubled on each expansion to
   keep realloc inexpensive. */
static idx_t   capacity        = 0;   /* current array capacity (>= n+1) */
static seq_t * a_arr          = NULL;/* a(n) values */
static seq_t * part_arr       = NULL;/* partition numbers P(n) */
static idx_t   max_a_computed = 0;   /* highest n for which a_arr is ready */
static idx_t   max_part       = 0;   /* highest n for which part_arr is ready */

/* Forward declaration of helpers; not used here. */
/* (You may include other helper header files as desired.) */

/* Ensure the internal arrays can hold indices up to n.  The
   capacity is doubled, and new entries are zeroed.  On first
   call the arrays are allocated and a(0)=1, P(0)=1 are set. */
static void ensure_capacity(idx_t n)
{
    if (n < capacity)
        return;

    size_t new_cap = capacity ? capacity << 1 : 1;
    while (new_cap <= n)
        new_cap <<= 1;

    a_arr    = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    part_arr = (seq_t *)realloc(part_arr, new_cap * sizeof(seq_t));
    if (!a_arr || !part_arr) abort();

    /* zero‑initialise newly allocated slots */
    for (idx_t i = capacity; i < new_cap; ++i) {
        a_arr[i]    = 0;
        part_arr[i] = 0;
    }
    capacity = new_cap;

    /* initialise base cases if this is the first allocation */
    if (capacity == new_cap && capacity > 1) {
        a_arr[0]    = (seq_t)1;
        part_arr[0] = (seq_t)1;
    }
}

/* Compute all partition numbers up to m (inclusive).  The
   partition array is filled using the pentagonal‑number
   recurrence.  All operations use seq_t additions and subtractions,
   which are safe because the intermediate positive sum always
   exceeds the negative sum. */
static void compute_partitions_up_to(idx_t m)
{
    if (m <= max_part)
        return;

    for (idx_t n = max_part + 1; n <= m; ++n) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = pent1 + k;          /* k*(3k+1)/2 */
            if (pent1 > n && pent2 > n)
                break;

            if (pent1 <= n) {
                seq_t val = part_arr[n - pent1];
                if (k & 1)
                    pos += val;
                else
                    neg += val;
            }
            if (pent2 <= n) {
                seq_t val = part_arr[n - pent2];
                if (k & 1)
                    pos += val;
                else
                    neg += val;
            }
        }
        part_arr[n] = pos - neg;
    }
    max_part = m;
}

/* Main sequence function.  It is memoised and works for any order
   of calls. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* a(0) = 1 */

    ensure_capacity(n);
    compute_partitions_up_to(n - 1);

    if (n <= max_a_computed)
        return a_arr[n];

    /* Compute missing values up to n. */
    for (idx_t m = max_a_computed + 1; m <= n; ++m) {
        seq_t sum   = 0;
        seq_t ratio = 1;               /* R(m,1) = 1 */
        for (idx_t k = 1; k <= m; ++k) {
            if (k > 1)                       /* update ratio */
                ratio *= (seq_t)(m - k + 1);

            seq_t term = a_arr[m - k];       /* a(n-k) */
            term *= (seq_t)k;                /* × k */
            term *= part_arr[k - 1];         /* × P(k-1) */
            term *= ratio;                   /* × R(m,k) */
            sum += term;
        }
        a_arr[m] = sum;
    }

    max_a_computed = n;
    return a_arr[n];
}
