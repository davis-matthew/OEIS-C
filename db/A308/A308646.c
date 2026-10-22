/*
 * A308646  a(n) = exp(1) * Sum_{k>=0} (-1)^k*k^(2*n)/k!.
 * https://oeis.org/A308646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..2*n} (-1)^k*Stirling2(2*n,k)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence is also a(n) = A000587(2*n). */
/* Computation is based on the recurrence for B_m(x) at x = –1:  */
/*     B_0 = 1,  B_m = –∑_{k=0}^{m-1} binom(m-1,k)*B_k   (m ≥ 1)  */
/* The values B_{2*n} equal a(n). The routine keeps a growing cache of B_m. */

#include <stdlib.h>

/* The build system supplies seq_t (signed integer type) and idx_t (unsigned). */

/* Cache of B_m values (0 ≤ m < B_size). */
static seq_t  *B_cache = NULL;   /* pointer to array of seq_t */
static idx_t   B_size   = 0;      /* number of entries actually stored */
static size_t  B_cap    = 0;      /* allocated capacity (in elements) */

/* Ensure that the array has at least need+1 elements (indices 0..need). */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need < B_cap)
        return;                  /* already big enough */

    size_t new_cap = B_cap ? B_cap << 1 : 1;
    while (new_cap <= (size_t)(need + 1))
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(B_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                  /* out of memory */

    B_cache = tmp;
    B_cap   = new_cap;
}

/* Compute B_m for all m up to 'need', using the recurrence. */
static void compute_B_up_to(idx_t need)
{
    if (B_size == 0) {            /* initialise first element */
        ensure_capacity(0);
        B_cache[0] = (seq_t)1;
        B_size = 1;
    }

    while (B_size <= need) {      /* compute next B_m */
        idx_t m   = B_size;       /* current m to compute */
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;     /* binom(m-1,0) = 1 */

        /* Σ_{k=0}^{m-1} binom(m-1,k) * B_k */
        for (idx_t k = 0; k < m; ++k) {
            if (k > 0) {
                /* bin = bin * (m - k) / k ; division is exact */
                bin = bin * (seq_t)(m - k);
                bin = bin / (seq_t)k;
            }
            sum += bin * B_cache[k];
        }

        B_cache[m] = -sum;        /* recurrence for B_m(-1) */
        ++B_size;
    }
}

/* Public function: a(n) = B_{2*n} */
seq_t a(idx_t n)
{
    idx_t m = 2 * n;               /* index of the table entry */

    if ((size_t)m >= B_cap)
        ensure_capacity(m);

    if ((idx_t)m >= B_size)
        compute_B_up_to(m);

    return B_cache[m];
}
