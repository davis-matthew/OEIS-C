/*
 * A398539  Square array T(n, m) read by antidiagonals downwards: T(n, m) is
 *          the m-th n-Knödel number, for n >= 1 and m >= 1.
 * https://oeis.org/A398539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, changed
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 120, u32 n <= 9955, u64 n <= 9955,
 *            u128 n <= 9955, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A398539 – n‑Knödel numbers.
 *
 *  For each n ≥ 1, T(n,m) is the m‑th composite integer m₀ such that
 *  Carmichael λ(m₀) divides (m₀ – n).  The sequence a(n) is obtained
 *  by reading the square array T(n,m) along anti‑diagonals downwards:
 *     a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), a(4)=T(1,3), …
 *
 *  This implementation pre‑computes all entries needed for n up to
 *  about 10 000 (diagonal sum ≤ 200).  The smallest prime factor sieve
 *  and Carmichael λ function are computed up to a fixed limit
 *  (5 000 000), which is sufficient for the required rows and columns.
 *
 *  All arithmetic uses only integer operators and the supplied types
 *  seq_t and idx_t.  No floating point, math.h or new/ C++ constructs.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Configuration constants                                   */
/* ------------------------------------------------------------ */
#define MAX_ROW   200          /* maximum row index that will be accessed   */
#define COL_MAX   200          /* maximum number of elements stored per row */
#define LIMIT     5000000      /* upper bound for computing Carmichael λ   */

/* ------------------------------------------------------------ */
/* Global state                                               */
/* ------------------------------------------------------------ */
static seq_t **rows          = 0;   /* rows[1..MAX_ROW][0..COL_MAX-1]          */
static size_t *row_cnt       = 0;   /* number of stored elements per row       */
static bool    initialized   = false;

/* ------------------------------------------------------------ */
/* Small helpers for 64‑bit arithmetic                        */
/* ------------------------------------------------------------ */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline uint64_t lcm_uint64(uint64_t a, uint64_t b)
{
    if (a == 0 || b == 0) return 0;
    return (a / gcd_uint64(a, b)) * b;
}

/* ------------------------------------------------------------ */
/* Pre‑computation: spf, Carmichael λ and the rows array     */
/* ------------------------------------------------------------ */
static void init_precompute(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate row containers */
    row_cnt = (size_t *)malloc((MAX_ROW + 1) * sizeof(size_t));
    rows    = (seq_t **)malloc((MAX_ROW + 1) * sizeof(seq_t *));
    for (idx_t i = 0; i <= MAX_ROW; ++i) {
        row_cnt[i] = 0;
        rows[i]    = (seq_t *)calloc(COL_MAX, sizeof(seq_t));
    }

    /* 1. Smallest prime factor sieve (linear) */
    uint32_t *spf = (uint32_t *)calloc((LIMIT + 1), sizeof(uint32_t));
    uint32_t *pr  = (uint32_t *)malloc((LIMIT / 10) * sizeof(uint32_t));
    size_t pr_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            pr[pr_cnt++] = i;
        }
        for (size_t j = 0; j < pr_cnt && (uint64_t)pr[j] * i <= LIMIT; ++j) {
            uint32_t v = pr[j] * i;
            spf[v] = pr[j];
            if (pr[j] == spf[i]) break;
        }
    }

    /* 2. Carmichael λ for all numbers up to LIMIT */
    uint64_t *lam = (uint64_t *)malloc((LIMIT + 1) * sizeof(uint64_t));
    lam[0] = 0;
    lam[1] = 1;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        uint32_t p   = spf[i];
        uint32_t exp = 0;
        uint32_t r   = i;
        while (r % p == 0) {
            r /= p;
            ++exp;
        }

        uint64_t lam_p;
        if (p == 2) {
            if (exp == 1) lam_p = 1;
            else if (exp == 2) lam_p = 2;
            else lam_p = 1ULL << (exp - 2);   /* 2^(exp-2) */
        } else {
            lam_p = 1;
            for (uint32_t k = 1; k < exp; ++k) lam_p *= (uint64_t)p; /* p^(exp-1) */
            lam_p *= (uint64_t)(p - 1);
        }
        lam[i] = lcm_uint64(lam[r], lam_p);
    }

    /* 3. Build the rows: for each composite m, add it to the
     *    appropriate rows n such that m - n is a multiple of λ(m).
     *    We stop once all rows are full. */
    size_t total_filled = 0;
    for (uint32_t m = 2; m <= LIMIT && total_filled < MAX_ROW * COL_MAX; ++m) {
        if (spf[m] == m) continue;          /* skip primes */
        uint64_t lambda = lam[m];
        if (lambda == 0) continue;           /* safety, shouldn't happen */

        /* n = m - k*lambda, k >= 1, n >= 1 */
        for (int64_t n = (int64_t)m - (int64_t)lambda; n >= 1;
             n -= (int64_t)lambda) {
            if (n > (int64_t)MAX_ROW) continue;
            size_t rn = (size_t)n;
            if (row_cnt[rn] >= COL_MAX) continue;
            rows[rn][row_cnt[rn]++] = (seq_t)m;
            ++total_filled;
            if (total_filled >= MAX_ROW * COL_MAX) break;
        }
    }

    /* Clean up temporary arrays */
    free(spf);
    free(pr);
    free(lam);
}

/* ------------------------------------------------------------ */
/* Public sequence function                                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1 */
    init_precompute();

    /* Find diagonal sum s such that s*(s-1)/2 ≥ n */
    idx_t s   = 2;                        /* start at diagonal 2 */
    idx_t cum = 1;                        /* cumulative count after diag 2 */
    while (cum < n) {
        ++s;
        cum += (s - 1);                   /* size of diagonal s */
    }

    /* Determine position inside this diagonal */
    idx_t prev = (s - 1) * (s - 2) / 2;   /* cumulative count before diag s */
    idx_t pos  = n - prev;                /* 1‑based position inside diag */

    /* Convert to 1‑based row and column indices */
    idx_t row = pos;                      /* row index (i) */
    idx_t col = s - pos;                  /* column index (j) */

    if (row == 0 || row > MAX_ROW || col == 0 || col > COL_MAX) {
        return (seq_t)0;                  /* safety, should not occur for n<=10000 */
    }

    return rows[row][col - 1];
}
