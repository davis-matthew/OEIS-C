/*
 * A383163  Expansion of e.g.f. log(1 - (exp(2*x) - 1)/2)^2 / 2.
 * https://oeis.org/A383163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum{k=2..n} 2^(n-k) * Stirling2(n,k) * |Stirling1(k,2)|
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- helper routine for resizing and initializing arrays ---------- */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    seq_t *tmp = (seq_t *)realloc(*arr, need * sizeof(seq_t));
    if (!tmp) abort();         /* cannot allocate */
    *arr = tmp;
    /* zero the newly added part */
    for (size_t i = *cap; i < need; ++i) (*arr)[i] = 0;
    *cap = need;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* 0 and 1 are 0 by definition */
    if (n == 0 || n == 1) return 0;

    /* static caches */
    static seq_t *fact_arr = NULL;  /* factorials */
    static size_t fact_cap = 0, fact_last = 0;
    static seq_t *pow2_arr = NULL;  /* powers of two */
    static size_t pow2_cap = 0, pow2_last = 0;
    static seq_t *hfact_arr = NULL; /* |Stirling1(k,2)| */
    static size_t hcap = 0, h_last = 0;
    static seq_t *S_arr = NULL;     /* current Stirling2 row: S[n][k] */
    static size_t S_cap = 0, S_last = 0;
    static seq_t *A_arr = NULL;     /* sequence values a[n] */
    static size_t A_cap = 0, A_last = 0;

    /* Drop calls that would request terms that are already known */
    if (A_last >= n) return A_arr[n];

    /* Make sure we have enough space for all demanded indices */
    size_t need = (size_t)n + 1;
    ensure_capacity(&fact_arr, &fact_cap, need);
    ensure_capacity(&pow2_arr, &pow2_cap, need);
    ensure_capacity(&hfact_arr, &hcap, need);
    ensure_capacity(&S_arr,    &S_cap,    need);
    ensure_capacity(&A_arr,    &A_cap,    need);

    /* ---- initialize base cases for each array if this is the first use ---- */
    if (fact_last == 0) { /* first allocation */
        fact_arr[0] = 1;
        fact_last = 0;
    }
    if (pow2_last == 0) {
        pow2_arr[0] = 1;
        pow2_last = 0;
    }
    if (h_last < 2) {      /* |Stirling1(2,2)| = 1 */
        hfact_arr[2] = 1;
        h_last = 2;
    }
    if (S_last == 0) {     /* S(1,1) = 1 */
        S_arr[0] = 0;
        S_arr[1] = 1;
        S_last = 1;
    }
    if (A_last == 0) {
        A_arr[0] = 0;
        A_arr[1] = 0;
        A_last = 1;
    }

    /* ---- grow factorials and powers of two up to n ---- */
    if (fact_last < n) {
        for (idx_t i = fact_last + 1; i <= n; ++i)
            fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
        fact_last = n;
    }

    if (pow2_last < n) {
        for (idx_t i = pow2_last + 1; i <= n; ++i)
            pow2_arr[i] = pow2_arr[i - 1] * 2;
        pow2_last = n;
    }

    /* ---- grow Stirling1 abs values for k = 3..n ---- */
    if (h_last < n) {
        for (idx_t k = h_last + 1; k <= n; ++k)
            hfact_arr[k] = ((seq_t)(k - 1)) * hfact_arr[k - 1] + fact_arr[k - 2];
        h_last = n;
    }

    /* ---- compute a(m) for each m from (A_last+1) up to n ---- */
    for (idx_t m = A_last + 1; m <= n; ++m) {
        /* Update a row of Stirling2 from n-1 to n */
        /* S_arr currently holds S(n-1, k). Compute S(n, k). */
        for (idx_t k = m; ; --k) {
            S_arr[k] = k * S_arr[k] + S_arr[k - 1];
            if (k == 1) break;
        }
        S_arr[0] = 0;
        S_last = m;

        /* Now compute a[m] using the formula */
        seq_t sum = 0;
        for (idx_t k = 2; k <= m; ++k) {
            seq_t term = S_arr[k] * hfact_arr[k];
            term = term * pow2_arr[m - k];
            sum += term;
        }
        A_arr[m] = sum;
    }
    A_last = n;

    return A_arr[n];
}
