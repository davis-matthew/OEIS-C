/*
 * A306098  Number of equivalence classes, modulo transposition, of
 *          non-symmetric plane partitions of n.
 * https://oeis.org/A306098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000219(n) - A005987(n))/2 where A000219(n) = number of
 *            plane partitions of n and A005987(n) = number of symmetric plane
 *            partitions of n. Plane partitions satisfy the recurrence n *
 *            A000219(n) = ∑_{k=1}^{n} (∑_{d|k} d²) * A000219(n-k) Symmetric
 *            plane partitions obey n * A005987(n) = ∑_{k=...
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 37, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* The following types are defined by the build environment. */
typedef seq_t seq_t;   /* arbitrary precision or unsigned integer type */
typedef idx_t idx_t;   /* idx_t is unsigned 64‑bit type */

static seq_t *P = NULL;       /* plane partitions A000219 */
static seq_t *S = NULL;       /* symmetric plane partitions A005987 */
static seq_t *Tp = NULL;      /* ∑_{d|k} d²   for plane partitions */
static seq_t *Ts = NULL;      /* ∑_{d|k} d*e(d) for symmetric partitions */
static idx_t _max_n = 0;

/* recompute all tables up to size n */
static void _grow(idx_t n)
{
    if (n <= _max_n) return;

    /* allocate new arrays of size n+1 (index 0..n) */
    seq_t *Pnew   = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *Snew   = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *Tpnew  = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *Tsnew  = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));

    /* compute divisor‑sum tables Tpnew and Tsnew for all k ≤ n */
    for (idx_t d = 1; d <= n; ++d)
    {
        seq_t d_sq  = (seq_t)d * (seq_t)d;
        seq_t ed;
        if (d & 1U)                      /* odd d */
            ed = 1;
        else
            ed = (d / 2U) / 2U;          /* floor(d/4) */
        seq_t d_ed = (seq_t)d * ed;

        for (idx_t m = d; m <= n; m += d)
        {
            Tpnew[m] += d_sq;
            Tsnew[m] += d_ed;
        }
    }

    /* compute plane partitions Pnew[0..n] */
    Pnew[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += Tpnew[k] * Pnew[i - k];
        Pnew[i] = sum / (seq_t)i;
    }

    /* compute symmetric plane partitions Snew[0..n] */
    Snew[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += Tsnew[k] * Snew[i - k];
        Snew[i] = sum / (seq_t)i;
    }

    /* free previous tables */
    free(P);
    free(S);
    free(Tp);
    free(Ts);

    /* store new tables */
    P  = Pnew;
    S  = Snew;
    Tp = Tpnew;
    Ts = Tsnew;
    _max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    _grow(n);
    seq_t diff = P[n] - S[n];
    return diff / 2;
}
