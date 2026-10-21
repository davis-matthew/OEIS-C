/*
 * A283986  a(n) = A002487(n-1) OR A002487(n), where OR is bitwise-or
 *          (A003986).
 * https://oeis.org/A283986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A002487(n-1) OR A002487(n) * A002487 is Stern's diatomic
 *            sequence: s(0)=0, s(1)=1, * s(2k)=s(k), s(2k+1)=s(k)+s(k+1)
 * Range:     u8 n <= 4690, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *stern_vals = NULL;     /* cache of Stern values, indices 0..max computed */
static idx_t  stern_size = 0;        /* number of valid entries (max index + 1) */
static idx_t  stern_cap  = 0;        /* allocated capacity */

/* Ensure stern_vals contains values up to index n (inclusive) */
static void ensure_stern(idx_t n)
{
    /* Nothing to do if already computed */
    if (stern_size > n)
        return;

    /* Initialize or grow the storage */
    if (stern_size == 0) {
        stern_cap = 2;                         /* at least indices 0 and 1 */
        seq_t *tmp = (seq_t *)realloc(stern_vals,
                                      sizeof(seq_t) * stern_cap);
        if (!tmp)
            return;                            /* allocation failed */
        stern_vals = tmp;
        stern_vals[0] = 0;
        stern_vals[1] = 1;
        stern_size = 2;                        /* indices 0 and 1 are valid */
    } else if (stern_cap <= n) {
        idx_t new_cap = stern_cap * 2;
        /* prevent overflow: if doubling would overflow or be too small */
        if (new_cap <= stern_cap || new_cap <= n)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(stern_vals,
                                      sizeof(seq_t) * new_cap);
        if (!tmp)
            return;                            /* allocation failed */
        stern_vals = tmp;
        stern_cap = new_cap;
    }

    /* Compute missing Stern values from stern_size up to n */
    for (idx_t i = stern_size; i <= n; ++i) {
        if ((i & 1) == 0) {                     /* even index */
            stern_vals[(size_t)i] = stern_vals[(size_t)(i >> 1)];
        } else {                                /* odd index */
            idx_t k = (i - 1) >> 1;
            stern_vals[(size_t)i] = stern_vals[(size_t)k] +
                                    stern_vals[(size_t)(k + 1)];
        }
    }
    /* Update the number of valid entries */
    stern_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out of sequence bounds, but harmless */

    /* Ensure we have Stern values for indices n-1 and n */
    ensure_stern(n);
    if (!stern_vals)
        return 0;                 /* allocation failure fallback */

    return stern_vals[(size_t)(n - 1)] | stern_vals[(size_t)n];
}
