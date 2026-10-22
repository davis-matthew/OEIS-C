/*
 * A357914  Iterated partial sums of the Moebius mu function, square array
 *          read by ascending antidiagonals.
 * https://oeis.org/A357914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by iterating partial sums of the Möbius
 *            μ‑function. Define T(1,k)=μ(k). For n>1, T(n,k)=Σ_{i=1..k}
 *            T(n-1,i). The sequence a(n) is the array T read by ascending
 *            antidiagonals with rows in descending order. Thus for each
 *            positive n, let D be the unique integer with D(D+1)/2 ≥ n. Th...
 * Range:     i8 n <= 82, i16 n <= 238, i32 n <= 718, i64 n <= 2515,
 *            i128 n <= 9109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  NOTE:  The types `seq_t` and `idx_t` are defined externally (as per
    the build).  The function uses only integer arithmetic and standard
    library headers. */

#include <stdlib.h>

static seq_t **T = NULL;          /* 2‑D table of iterated partial sums   */
static idx_t T_limit = 0;         /* current table size (>=1)              */

static void ensure_dim(idx_t limit)
{
    if (limit <= T_limit)
        return;

    /* ---- compute Möbius μ[1..limit] --------------------------------- */
    seq_t *mu = (seq_t*)calloc(limit + 1, sizeof(seq_t));
    int   *is_comp = (int*)calloc(limit + 1, sizeof(int));
    int   *primes  = (int*)malloc(limit * sizeof(int));
    int    pcnt = 0;

    mu[1] = 1;
    for (idx_t n = 2; n <= limit; ++n) {
        if (!is_comp[n]) {
            primes[pcnt++] = (int)n;
            mu[n] = -1;
        }
        for (int p = 0; p < pcnt; ++p) {
            idx_t prod = n * (idx_t)primes[p];
            if (prod > limit)
                break;
            is_comp[prod] = 1;
            if (n % primes[p] == 0) {
                mu[prod] = 0;
                break;
            } else {
                mu[prod] = -mu[n];
            }
        }
    }

    free(is_comp);
    free(primes);

    /* ---- allocate new table ----------------------------------------- */
    seq_t **new_T = (seq_t**)malloc((limit + 1) * sizeof(seq_t*));
    for (idx_t i = 1; i <= limit; ++i)
        new_T[i] = (seq_t*)calloc(limit + 1, sizeof(seq_t));

    /* ---- fill row 1 with μ ------------------------------------------ */
    for (idx_t j = 1; j <= limit; ++j)
        new_T[1][j] = mu[j];

    /* ---- fill remaining rows via cumulative sums --------------------- */
    for (idx_t i = 2; i <= limit; ++i) {
        seq_t cum = 0;
        for (idx_t j = 1; j <= limit; ++j) {
            cum += new_T[i-1][j];
            new_T[i][j] = cum;
        }
    }

    /* ---- free old table --------------------------------------------- */
    if (T) {
        for (idx_t i = 1; i <= T_limit; ++i)
            free(T[i]);
        free(T);
    }
    free(mu);

    T      = new_T;
    T_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* out of bounds – not used by the OEIS */

    /* find diagonal D: smallest with D*(D+1)/2 >= n */
    idx_t D = 1;
    while (D * (D + 1) / 2 < n)
        ++D;

    idx_t prev  = (D - 1) * D / 2;          /* total elements before this diagonal */
    idx_t pos   = n - prev - 1;             /* 0‑based offset on the diagonal */
    idx_t i = D - pos;                      /* row index (descending order) */
    idx_t j = pos + 1;                      /* column index (ascending order) */

    ensure_dim(D);                           /* make sure T is large enough */

    return T[i][j];
}
