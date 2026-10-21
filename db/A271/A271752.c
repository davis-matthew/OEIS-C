/*
 * A271752  Number of set partitions of [n+1] such that n is the largest
 *          element of the last block.
 * https://oeis.org/A271752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Bell(n) – 3*Bell(n‑1) (for n ≥ 2), a(1) = 0 * Bell
 *            numbers are computed with the recurrence * B[0] = 1 * B[N] =
 *            Σ_{k=0}^{N‑1} C(N‑1, k) * B[k] (N ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for Bell numbers */
static seq_t *Bell = NULL;     /* array of Bell numbers, Bell[i] = B_i   */
static idx_t BellSize = 0;     /* current allocated size (BellSize = allocated count) */

/* ensure that Bell[0..max] are computed */
static void ensure_bell(idx_t max)
{
    if (BellSize > max) return;          /* already have them */
    /* enlarge the array */
    idx_t newSize = max + 1;
    seq_t *tmp = (seq_t *)realloc(Bell, sizeof(seq_t) * newSize);
    if (!tmp) abort();                   /* allocation failure */
    Bell = tmp;

    /* compute new entries */
    for (idx_t n = BellSize; n <= max; ++n)
    {
        if (n == 0) {                    /* B[0] = 1 */
            Bell[0] = (seq_t)1;
            continue;
        }
        seq_t sum  = (seq_t)0;
        seq_t binom = (seq_t)1;           /* C(n-1, 0) */
        idx_t nMinus1 = n - 1;
        /* sum_{k=0}^{n-1} C(n-1, k) * B[k] */
        for (idx_t k = 0; k < n; ++k)
        {
            sum += binom * Bell[k];
            if (k + 1 < n)                /* compute C(n-1, k+1) */
            {
                seq_t num = (seq_t)(nMinus1 - k);
                seq_t den = (seq_t)(k + 1);
                binom = (binom * num) / den;
            }
        }
        Bell[n] = sum;
    }

    BellSize = newSize;
}

/* main function required by the specification */
seq_t a(idx_t n)
{
    if (n <= 1)                /* a(1) = 0, and a(0) is unused but safe */
        return (seq_t)0;

    /* we need Bell[n] and Bell[n-1] */
    ensure_bell(n);

    seq_t two   = (seq_t)2;
    seq_t three = (seq_t)3;

    /* a(n) = 2 * Bell[n] – 3 * Bell[n‑1] */
    return two * Bell[n] - three * Bell[n - 1];
}
