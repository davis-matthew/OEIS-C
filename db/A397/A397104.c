/*
 * A397104  Triangle read by rows: T(n, k) = n + A275314(k) - 1, for 1 <= k <=
 *          n and gcd(n, k) = 1.
 * https://oeis.org/A397104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) = T(r,k) where r is the smallest integer with * sum_{i=1}^r
 *            φ(i) ≥ n, k is the pos‑th integer coprime to r * (pos = n –
 *            sum_{i=1}^{r-1} φ(i)), and * T(r,k) = r + A275314(k) – 1. *
 *            (A275314(k) = ∑_{p^e‖k} (e·p – e) + 1.)
 * Range:     u8 n <= 5152, u16 n <= 9754, u32 n <= 9754, u64 n <= 9754,
 *            u128 n <= 9754, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h" /* static seq_t gcd(seq_t a, seq_t b) */

static idx_t *phi_arr   = NULL;
static idx_t *cum_arr   = NULL;
static idx_t *spf_arr   = NULL;
static seq_t *A275_arr  = NULL;
static size_t current_limit = 0;

/* --------------------------------------------------------------------- */
/* pre–compute φ, cumulative φ, smallest prime factor, and A275314 up   */
/* to at least n (inclusive).  On extension we free the old tables and  */
/* recompute everything – the ranges are small (n ≤ ~10⁴).               */
/* --------------------------------------------------------------------- */
static void precompute_until(idx_t n)
{
    if (n <= current_limit)
        return;

    size_t lim = (size_t)n + 1;                  /* allocate 0..n */

    idx_t  *phi = (idx_t *)malloc(lim * sizeof(idx_t));
    idx_t  *cum = (idx_t *)malloc(lim * sizeof(idx_t));
    idx_t  *spf = (idx_t *)malloc(lim * sizeof(idx_t));
    seq_t  *A275= (seq_t *)malloc(lim * sizeof(seq_t));

    if (!phi || !cum || !spf || !A275) {
        /* memory allocation failed – keep previous data */
        free(phi); free(cum); free(spf); free(A275);
        return;
    }

    /* initialise φ */
    for (idx_t i = 0; i < lim; ++i) phi[i] = i;
    phi[0] = 0;
    for (idx_t i = 2; i < lim; ++i) {
        if (phi[i] == i) {                     /* i is prime */
            for (idx_t j = i; j < lim; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    /* cumulative sum of φ */
    cum[0] = 0;
    for (idx_t i = 1; i < lim; ++i)
        cum[i] = cum[i - 1] + phi[i];

    /* smallest prime factor array */
    for (idx_t i = 0; i < lim; ++i) spf[i] = 0;
    for (idx_t i = 2; i < lim; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((idx_t)i * i < lim) {
                for (idx_t j = i * i; j < lim; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* A275314(k) for 0..n */
    A275[0] = 0;
    A275[1] = 1;
    for (idx_t m = 2; m < lim; ++m) {
        idx_t t = m;
        seq_t sum_ep = 0, sum_e = 0;
        while (t > 1) {
            idx_t p = spf[t];
            seq_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            sum_ep += e * p;
            sum_e += e;
        }
        A275[m] = sum_ep - sum_e + 1;
    }

    /* replace old tables */
    free(phi_arr); free(cum_arr); free(spf_arr); free(A275_arr);
    phi_arr   = phi;
    cum_arr   = cum;
    spf_arr   = spf;
    A275_arr  = A275;
    current_limit = n;
}

/* --------------------------------------------------------------------- */
/*  a(n) – return the n-th term of A397104 (flattened triangle).       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* undefined, safe fallback */

    precompute_until(n);

    /* binary search for the row r such that cum[r‑1] < n ≤ cum[r] */
    idx_t low = 1, high = current_limit;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (cum_arr[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row number */
    idx_t pos = n - cum_arr[r - 1];    /* position within this row (1‑based) */

    /* find the pos‑th integer k ≤ r coprime to r */
    idx_t cnt = 0, k = 0;
    for (idx_t kk = 1; kk <= r; ++kk) {
        if (gcd((seq_t)kk, (seq_t)r) == 1) {
            ++cnt;
            if (cnt == pos) {
                k = kk;
                break;
            }
        }
    }

    /* compute T(r,k) = r + A275314(k) – 1 */
    seq_t result = (seq_t)r + A275_arr[k] - 1;
    return result;
}
