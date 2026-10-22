/*
 * A335666  a(n) is the sum, over all overpartitions of n, of the overlined
 *          parts.
 * https://oeis.org/A335666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] (Prod_{k>=1} (1+q^k)/(1-q^k)) * Sum_{m>=1} m
 *            q^m/(1+q^m)
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes a(n) by first generating the
 * overpartition counts f(k) = number of overpartitions of k,
 * then the auxiliary series h(t) = Σ_{d|t} d * (-1)^{t/d-1},
 * and finally a convolution: a(n) = Σ_{t=0}^n f(t)*h(n-t). */

#include <stdlib.h>

static seq_t *f_arr = NULL;   /* overpartition counts f[0..N] */
static seq_t *h_arr = NULL;   /* auxiliary series h[0..N] */
static seq_t *a_arr = NULL;   /* final sequence a[0..N] */
static size_t computed_N = 0; /* maximum n for which arrays are valid */

/* ------------------------------------------------------------------ */
/* Compute f(k), h(t), and a(n) for all values up to N.               */
static void compute_arrays(size_t N)
{
    if (N == computed_N)
        return; /* already have the needed data */

    /* Allocate temporary arrays for f(k) computation */
    seq_t *oldf = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *newf = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= N; ++i) {
        oldf[i] = (i == 0) ? 1 : 0;
        newf[i] = 0;
    }

    /* Overpartition DP:  f[n] = 1 + 2 Σ_{j>=1} f[n-j*k] for each part size k */
    for (size_t k = 1; k <= N; ++k) {
        /* copy old values */
        for (size_t i = 0; i <= N; ++i)
            newf[i] = oldf[i];

        /* add contributions of multiples of k */
        for (size_t j = 1; ; ++j) {
            size_t kk = j * k;
            if (kk > N)
                break;
            for (size_t n = kk; n <= N; ++n) {
                seq_t val = oldf[n - kk];
                newf[n] += val;
                newf[n] += val;          /* multiply by 2 */
            }
        }
        /* swap buffers */
        seq_t *tmp = oldf;
        oldf = newf;
        newf = tmp;
    }

    /* Allocate final arrays f_arr, h_arr, a_arr */
    seq_t *f = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *h = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *a = (seq_t *)malloc((N + 1) * sizeof(seq_t));

    /* Copy computed f into public array */
    for (size_t i = 0; i <= N; ++i)
        f[i] = oldf[i];

    free(oldf);
    free(newf);

    /* ---- compute h(t) --------------------------------------------- */
    h[0] = 0;
    for (size_t t = 1; t <= N; ++t) {
        seq_t pos = 0, neg = 0;
        for (size_t d = 1; d * d <= t; ++d) {
            if (t % d == 0) {
                size_t d1 = d;
                size_t d2 = t / d;
                size_t exp1 = t / d1;   /* this equals d2 */
                if (exp1 & 1)
                    pos += d1;
                else
                    neg += d1;

                if (d2 != d1) {
                    size_t exp2 = t / d2; /* this equals d1 */
                    if (exp2 & 1)
                        pos += d2;
                    else
                        neg += d2;
                }
            }
        }
        /* h[t] = pos - neg  (pos ≥ neg always holds) */
        h[t] = pos - neg;
    }

    /* ---- compute a(n) via convolution ------------------------------ */
    a[0] = 0;
    for (size_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t t = 0; t <= n; ++t)
            sum += f[t] * h[n - t];
        a[n] = sum;
    }

    /* Replace previous global arrays (free old ones) */
    free(f_arr);
    free(h_arr);
    free(a_arr);
    f_arr = f;
    h_arr = h;
    a_arr = a;
    computed_N = N;
}

/* ------------------------------------------------------------------ */
/* Public function required by the task.                               */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for index 0 in OEIS terms */
    size_t N = (size_t)n;
    compute_arrays(N);
    return a_arr[N];
}
