/*
 * A282011  Number T(n,k) of k-element subsets of [n] having an even sum;
 *          triangle T(n,k), n>=0, 0<=k<=n, read by rows.
 * https://oeis.org/A282011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row satisfying r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2; k = n - r*(r+1)/2. T(r,k) = Σ_{j=0..⌊(r+1)/4⌋}
 *            C(⌈r/2⌉,2j) * C(⌊r/2⌋,k-2j)
 * Range:     u8 n <= 82, u16 n <= 218, u32 n <= 682, u64 n <= 2445,
 *            u128 n <= 8974, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes from the build provide the types seq_t and idx_t.  */

#include <stdbool.h>

/* ---------- small helper: binomial with idx_t arguments ---------- */
static inline seq_t binomial_simple(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;               /* use symmetry nCk = nC(n-k) */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res * (n - k + i) is always divisible by i */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* ---------- triangular number start index of row r (0‑based) ---------- */
static inline idx_t tri_start(idx_t r)
{
    return r * (r + 1) / 2;
}

/* ---------- compute T(r,k) : number of k‑subsets of [r] with even sum ---------- */
static inline seq_t t_even_sum(idx_t r, idx_t k)
{
    idx_t ceil_half = (r + 1) / 2;   /* ⌈r/2⌉  */
    idx_t floor_half = r / 2;        /* ⌊r/2⌋  */
    idx_t jmax = (r + 1) / 4;        /* ⌊(r+1)/4⌋  */
    seq_t result = 0;

    for (idx_t j = 0; j <= jmax; ++j) {
        idx_t odds = 2 * j;
        if (odds > ceil_half) break;          /* further terms impossible   */
        if (odds > k) continue;               /* would make evens negative   */
        idx_t evens = k - odds;
        if (evens > floor_half) continue;     /* not enough evens left       */

        seq_t term = binomial_simple(ceil_half, odds) *
                     binomial_simple(floor_half, evens);
        result += term;
    }
    return result;
}

/* ---------- main entry: a(n) returns the flattened triangle entry ---------- */
seq_t a(idx_t n)
{
    /* Find row r such that tri_start(r) <= n < tri_start(r+1). */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if (tri_start(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r = low;
    idx_t start = tri_start(r);
    idx_t k = n - start;          /* column index within the row */

    return t_even_sum(r, k);
}
