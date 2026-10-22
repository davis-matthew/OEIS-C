/*
 * A341062  Sequence whose partial sums give A000005.
 * https://oeis.org/A341062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = τ(n) − τ(n−1) for n > 1, a(1)=1
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* τ(n) is the number of divisors of n.  The code caches divisor counts
   and smallest prime factors to allow efficient evaluation for all n. */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Global state ---------- */
static uint64_t *spf = NULL;      /* smallest prime factor array          */
static idx_t    spf_cap = 0;      /* size of spf (indexes 0..spf_cap-1)   */

static seq_t *tau = NULL;         /* τ values, 1‑indexed                  */
static idx_t  tau_cap = 0;        /* size of tau (indexes 0..tau_cap-1)   */
static idx_t  tau_last = 0;       /* highest index computed so far        */

/* ---------- Helper: initialize spf array ---------- */
static void init_spf(idx_t n)
{
    if (spf_cap != 0) return;                /* already initialized */
    size_t new_cap = (size_t)(n + 1);
    uint64_t *new_spf = (uint64_t *)calloc(new_cap, sizeof(uint64_t));
    if (!new_spf) return;                    /* allocation failure */
    spf = new_spf;
    spf_cap = new_cap;
}

/* ---------- Helper: ensure spf is computed up to n ---------- */
static void ensure_spf(idx_t n)
{
    if (spf_cap > n) return;                /* already up to date */

    size_t new_cap = (size_t)(n + 1);
    uint64_t *new_spf = (uint64_t *)realloc(spf, new_cap * sizeof(uint64_t));
    if (!new_spf) return;                   /* allocation failure */
    for (idx_t i = spf_cap; i <= n; ++i) new_spf[i] = 0;   /* initialise new part */

    /* recompute spf from scratch up to n */
    for (idx_t i = 2; i <= n; ++i) new_spf[i] = 0;
    for (idx_t i = 2; i <= n / i; ++i) {
        if (new_spf[i] == 0) {              /* i is prime */
            new_spf[i] = i;
            for (idx_t j = i * i; j <= n; j += i) {
                if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }
    for (idx_t i = 2; i <= n; ++i) if (new_spf[i] == 0) new_spf[i] = i;

    spf = new_spf;
    spf_cap = new_cap;
}

/* ---------- Helper: initialize tau array ---------- */
static void init_tau(void)
{
    if (tau_cap != 0) return;                /* already initialized */
    size_t new_cap = 2;                       /* at least space for index 1 */
    tau = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!tau) return;                        /* allocation failure */
    tau[0] = 0;
    tau[1] = 1;
    tau_cap = new_cap;
    tau_last = 1;
}

/* ---------- Helper: compute τ(k) using spf ---------- */
static seq_t tau_of(idx_t k)
{
    seq_t result = 1;
    idx_t m = k;
    while (m > 1) {
        uint64_t p = spf[m];
        idx_t cnt = 0;
        while (m % p == 0) {
            m /= p;
            ++cnt;
        }
        result = result * ((seq_t)cnt + 1);
    }
    return result;
}

/* ---------- Helper: ensure τ values are computed up to n ---------- */
static void ensure_tau(idx_t n)
{
    init_tau();
    ensure_spf(n);
    if (n < tau_cap) return;                /* already computed */

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_tau = (seq_t *)realloc(tau, new_cap * sizeof(seq_t));
    if (!new_tau) return;                   /* allocation failure */
    tau = new_tau;

    for (idx_t i = tau_last + 1; i <= n; ++i) {
        tau[i] = tau_of(i);
    }
    tau_cap = new_cap;
    tau_last = n;
}

/* ---------- Required function ---------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;      /* a(1) = 1 */

    ensure_tau(n);                          /* ensures τ(n) and τ(n‑1) are ready */

    seq_t tau_n   = tau[n];
    seq_t tau_nm1 = tau[n - 1];
    return tau_n - tau_nm1;
}
