/*
 * A327924  Square array read by ascending antidiagonals: T(m,n) is the number
 *          of non-isomorphic groups G such that G is the semidirect product
 *          of C_m and C_n, where C_m is a normal subgroup of G and C_n is a
 *          subgroup of G.
 * https://oeis.org/A327924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(m,n) = Σ_{d|n} (#x ∈ (ℤ/mℤ)* with ord(x,m)=d) / φ(d), with
 *            (m,n) read by ascending antidiagonals in the square array
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd, make_zero_seq_array, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/phi.h"   /* seq_t phi(idx_t m); */
#include "../utils/make_zero_seq_array.h"

#define MAX_M 200   /* sufficient for n ≤ 10 000 */

/* Pre‑computed Euler totient values for 0 … MAX_M. */
static seq_t phi_table[MAX_M + 1];
static bool phi_computed[MAX_M + 1] = { false };

static seq_t get_phi(int m)
{
    if (!phi_computed[m]) {
        phi_table[m] = phi((idx_t)m);
        phi_computed[m] = true;
    }
    return phi_table[m];
}

/* For each m, store cnt[ord] = number of units modulo m of exact order ord. */
static seq_t *cnt_table[MAX_M + 1] = { NULL };
static bool cnt_computed[MAX_M + 1] = { false };

static void compute_cnt(int m)
{
    if (cnt_computed[m]) return;

    seq_t *cnt = make_zero_seq_array((idx_t)m + 1);

    for (idx_t x = 1; x < (idx_t)m; ++x) {
        if (gcd((seq_t)x, (seq_t)m) != 1) continue;

        idx_t ord = 1;
        seq_t t = (seq_t)x;
        /* compute the multiplicative order of x modulo m */
        while (t != (seq_t)1) {
            t = (t * (seq_t)x) % (seq_t)m;
            ++ord;
        }
        cnt[(int)ord] += (seq_t)1;
    }

    cnt_table[m] = cnt;
    cnt_computed[m] = true;
}

/* Compute T(m,n) for positive integer m and n. */
static seq_t compute_T(seq_t m, seq_t n)
{
    if (m == (seq_t)1) return (seq_t)1;   /* C₁ is trivial */

    int mi = (int)m;   /* mi ≤ MAX_M for the problem limits */
    compute_cnt(mi);
    seq_t *cnt = cnt_table[mi];

    seq_t result = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        if (d > (idx_t)mi) continue;
        seq_t f = cnt[(int)d];
        if (f == (seq_t)0) continue;
        seq_t phi_d = get_phi((int)d);
        result += f / phi_d;
    }
    return result;
}

/* Map the 1‑based index n to the coordinates (m,n) using ascending antidiagonals
   and return T(m,n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below the OEIS offset, undefined */

    /* Find the first s such that (s-1)*s/2 ≥ n; s = diagonal index + 1. */
    idx_t s = 1;
    while (( (__uint128_t)(s - 1) * s ) / 2 < n) ++s;

    idx_t k = s - 1;                                    /* sum m+n = k+1 */
    idx_t prev_sum = (k * (k - 1)) / 2;                 /* entries before this diagonal */
    idx_t offset = n - prev_sum - 1;                    /* 0‑based position in the diagonal */

    idx_t m_val = k - offset;                           /* m increases from 1 */
    idx_t n_val = offset + 1;                           /* corresponding n */

    return compute_T((seq_t)m_val, (seq_t)n_val);
}
