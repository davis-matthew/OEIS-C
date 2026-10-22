/*
 * A309428  Irregular triangle read by rows: T(n,k) is the multiplicative
 *          order of {{A038566(n,k), 1}, {0, 1}} modulo n, n >= 1, 1 <= k <=
 *          A000010(n).
 * https://oeis.org/A309428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th entry of the irregular triangle T(m,k)
 *            described * in OEIS A309428. The triangle entries are computed
 *            as the multiplicative * order of the matrix [[r,1],[0,1]] modulo
 *            m, where r is the k-th reduced * residue modulo m. The sequence
 *            a is the triangle read by rows
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies required from the build: gcd.h, mod_pow.h */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

/* ---------- Precomputation of φ(n) and cumulative sums ---------- */
#define MAX_LIMIT 200000            /* enough to cover indices up to a few
                                      * 100 million (more than required)    */
static idx_t *phi_arr = NULL;
static idx_t *cum_phi = NULL;
static bool precomp_done = false;

static void precompute_phi(void)
{
    phi_arr = (idx_t *)malloc((MAX_LIMIT + 1) * sizeof(idx_t));
    cum_phi = (idx_t *)malloc((MAX_LIMIT + 1) * sizeof(idx_t));
    if (!phi_arr || !cum_phi) exit(1);  /* memory failure */

    for (idx_t i = 0; i <= MAX_LIMIT; ++i) phi_arr[i] = i;
    phi_arr[0] = 0;
    phi_arr[1] = 1;

    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (phi_arr[i] == i) {                 /* i is prime */
            for (idx_t j = i; j <= MAX_LIMIT; j += i)
                phi_arr[j] -= phi_arr[j] / i;
        }
    }

    cum_phi[0] = 0;
    for (idx_t i = 1; i <= MAX_LIMIT; ++i)
        cum_phi[i] = cum_phi[i - 1] + phi_arr[i];

    precomp_done = true;
}

/* ---------- Helper: k-th reduced residue modulo n ----------
 * Since n is at most about MAX_LIMIT, a simple scan is fine. */
static idx_t kth_coprime(idx_t n, idx_t k)
{
    idx_t cnt = 0;
    for (idx_t x = 1; x < n; ++x) {
        if (gcd((seq_t)x, (seq_t)n) == (seq_t)1) {
            ++cnt;
            if (cnt == k) return x;
        }
    }
    return 1;   /* should never happen for valid input */
}

/* ---------- Helper: multiplicative order of a modulo m ----------
 * a and m are coprime.  m > 0, m fits in 64-bit. */
static seq_t multiplicative_order(seq_t a, seq_t m)
{
    /* Compute φ(m) via factorisation of m */
    seq_t phi_m = m;
    idx_t temp = (idx_t)m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            phi_m /= p;
            phi_m *= (p - 1);
        }
    }
    if (temp > 1) {
        phi_m /= temp;
        phi_m *= (temp - 1);
    }

    idx_t phi_u = (idx_t)phi_m;        /* φ(m) is small (<= 4·10^4) */

    for (idx_t e = 1; e <= phi_u; ++e) {
        if (phi_u % e == 0) {
            if (mod_pow(a, (uint64_t)e, m) == (seq_t)1)
                return (seq_t)e;
        }
    }
    return phi_m;   /* fallback, should never reach here */
}

/* ---------- Main function --------------------------------------- */
seq_t a(idx_t n)
{
    if (!precomp_done) precompute_phi();

    /* Find the row r containing the n-th term */
    idx_t low = 1, high = MAX_LIMIT, r = MAX_LIMIT;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (cum_phi[mid] >= n) {
            r = mid;
            high = mid - 1;
        } else
            low = mid + 1;
    }

    /* Position within the r-th row */
    idx_t k = n - cum_phi[r - 1];

    /* k-th reduced residue modulo r */
    idx_t r_val = kth_coprime(r, k);

    /* Compute the matrix multiplicative order T(r, r_val) */
    seq_t result;
    if (r == 1) {
        result = (seq_t)1;                 /* special case: a(1) = 1 */
    } else if (r_val % r == 1) {
        result = (seq_t)r;                 /* r ≡ 1 (mod r) → order = r */
    } else {
        /* m = r * (r_val - 1) and compute ord(r_val, m) */
        seq_t m = (seq_t)r * ((seq_t)r_val - 1);
        result = multiplicative_order((seq_t)r_val, m);
    }

    return result;
}
