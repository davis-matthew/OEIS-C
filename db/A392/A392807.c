/*
 * A392807  Number of plane partitions of n with 3 parts.
 * https://oeis.org/A392807
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*T(n-3) + T(n-5), where T(m) = number of non‑negative
 *            integer solutions of a + 2*b + 3*c = m
 * Range:     u8 n <= 28, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *Tarr = NULL;   /* Cache for T(m), m >= 0 */
static idx_t Tsize = 0;      /* Current size of Tarr (number of stored entries) */

/* Ensure Tarr contains values up to and including m; allocate/realloc if needed. */
static void ensure_T(idx_t m)
{
    if (Tsize > m) return;               /* Already have needed range */
    size_t newsize = (size_t)m + 1;
    seq_t *newarr = (seq_t *)realloc(Tarr, newsize * sizeof(seq_t));
    if (!newarr) abort();                /* Allocation failure */
    /* Compute T values from current size up to newsize-1 */
    for (idx_t k = Tsize; k < newsize; ++k) {
        seq_t sum = 0;
        idx_t maxc = k / 3;
        for (idx_t c = 0; c <= maxc; ++c) {
            idx_t rem = k - 3 * c;       /* remaining after choosing c */
            sum += (seq_t)(rem / 2) + 1; /* number of b values for this c */
        }
        newarr[k] = sum;
    }
    Tarr = newarr;
    Tsize = newsize;
}

/* Plane partitions of n with 3 parts; n >= 3. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* undefined below offset, return 0 */

    idx_t m1 = n - 3;                   /* argument for T in the first term */
    idx_t m2 = (n >= 5) ? n - 5 : (idx_t)-1;  /* argument for the second term */
    ensure_T(m1);
    seq_t t1 = Tarr[m1];
    seq_t t2 = 0;
    if (m2 != (idx_t)-1) {
        ensure_T(m2);
        t2 = Tarr[m2];
    }
    return t1 * 3 + t2;
}
