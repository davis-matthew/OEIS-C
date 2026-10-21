/*
 * A289847  p-INVERT of the primes (A000040), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = coefficient of x^n in the Maclaurin series of * T(x) =
 *            (-p(0) + 1/p(S(x))) / x * where p(S)=1 - S - S² and S(x)=∑_{k≥0}
 *            prime_k · x^{k+1}. * Thus a(n)=b_{n+1} where b satisfies * B(x)
 *            = 1/(1 - S(x) - S(x)²) = ∑_{m≥0} b_m x^m. * Equipped with the
 *            recurrence: * for m≥1: * C_m = ∑_{j=0}^{m-...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------- Standard library headers -------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ----- Macro to cast idx_t to size_t safely ----- */
#define IDX_TO_SIZE(x) ((size_t)(x))

/* ---- Static storage for primes and series coefficients ---- */
static idx_t max_n_computed = (idx_t)-1;          /* largest n already computed for a(n) */
static seq_t *b_arr = NULL;                       /* coefficients of B(x), size up to n+2 */
static size_t b_cap = 0;                          /* allocated capacity for b_arr */

static seq_t *c_arr = NULL;                       /* auxiliary array C(x), same size as b_arr */
static size_t c_cap = 0;                          /* allocated capacity for c_arr */

static uint64_t *prime_arr = NULL;                /* list of primes, size at least needed */
static size_t prime_cap = 0;                      /* allocated size for prime_arr */

/* ----- Helper: generate at least 'cnt' primes (0‑indexed) ---- */
static void generate_primes_up_to(size_t cnt)
{
    if (prime_cap >= cnt)
        return;

    /* Estimate an upper bound for the (cnt‑th) prime using n(log n + log log n) + 3 */
    size_t limit;
    if (cnt < 6) {
        limit = 15;          /* small safe bound */
    } else {
        double n = (double)cnt;
        double x = n * (log(n) + log(log(n))) + 3.0;
        limit = (size_t)(x + 1.0);
    }

    while (true) {
        bool *comp = (bool *)calloc(limit + 1, sizeof(bool));
        if (!comp) abort();

        size_t found = 0;
        if (!prime_arr) {
            prime_arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
            if (!prime_arr) abort();
        } else if (prime_cap < cnt) {
            uint64_t *tmp = (uint64_t *)realloc(prime_arr, cnt * sizeof(uint64_t));
            if (!tmp) abort();
            prime_arr = tmp;
        }

        for (size_t i = 2; i <= limit; ++i) {
            if (!comp[i]) {
                if (found < cnt)
                    prime_arr[found++] = (uint64_t)i;
                if (i <= limit / i) {
                    for (size_t j = i * i; j <= limit; j += i)
                        comp[j] = true;
                }
            }
        }
        free(comp);

        if (found >= cnt) {
            prime_cap = cnt;
            break;
        }
        limit *= 2;   /* increase limit and retry */
    }
}

/* ----- Helper: ensure b_arr and c_arr have capacity at least 'need' ---- */
static void ensure_b_capacity(size_t need)
{
    if (b_cap < need) {
        seq_t *tmp = (seq_t *)realloc(b_arr, need * sizeof(seq_t));
        if (!tmp) abort();
        b_arr = tmp;
        b_cap = need;
    }
    if (c_cap < need) {
        seq_t *tmpc = (seq_t *)realloc(c_arr, need * sizeof(seq_t));
        if (!tmpc) abort();
        c_arr = tmpc;
        c_cap = need;
    }
}

/* -------------- Main function ---------------- */
seq_t a(idx_t n)
{
    /* we need coefficients up to index n+1 */
    idx_t need = n + 1;
    size_t needed_cap = IDX_TO_SIZE(need) + 1;   /* indexes 0 .. need */

    /* Ensure primes up to need (i.e., at least need primes) */
    generate_primes_up_to((size_t)need);

    /* Ensure storage for b and c */
    ensure_b_capacity(needed_cap);

    /* Initialise if first call */
    if (max_n_computed == (idx_t)-1) {
        b_arr[0] = (seq_t)1;
        c_arr[0] = (seq_t)0;
        max_n_computed = 0;
    }

    /* Compute coefficients up to 'need' if necessary */
    for (idx_t k = IDX_TO_SIZE(max_n_computed + 1); k <= need; ++k) {
        seq_t sumC = 0;
        seq_t sumD = 0;
        /* C_k = sum_{j=0}^{k-1} prime_j * b_{k-j-1} */
        for (idx_t j = 0; j < k; ++j) {
            sumC += prime_arr[IDX_TO_SIZE(j)] * b_arr[IDX_TO_SIZE(k - j - 1)];
        }
        c_arr[IDX_TO_SIZE(k)] = sumC;

        /* D_k = sum_{j=0}^{k-1} prime_j * C_{k-j-1} */
        for (idx_t j = 0; j < k; ++j) {
            sumD += prime_arr[IDX_TO_SIZE(j)] * c_arr[IDX_TO_SIZE(k - j - 1)];
        }

        /* b_k = C_k + D_k  (delta term is 0 for k>0) */
        b_arr[IDX_TO_SIZE(k)] = sumC + sumD;
    }

    max_n_computed = need;

    /* a(n) = b_{n+1} */
    return b_arr[IDX_TO_SIZE(n + 1)];
}
