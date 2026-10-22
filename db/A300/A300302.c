/*
 * A300302  Square array T(n, k) (n >= 1, k >= 1) read by antidiagonals
 *          upwards: T(n, k) is the k-th positive number whose binary
 *          representation contains the binary representation of n as a
 *          substring.
 * https://oeis.org/A300302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A300302:
   a(n) is the entry in the square array T(n,k) read by antidiagonals upwards.
   For a given index n (n ≥ 1) let t be the smallest integer with
      t*(t+1)/2 ≥ n                     (t is the diagonal number)
   Let pos = n - (t-1)*t/2               (1‑based position within that diagonal)
   Then
      n_val = t + 1 - pos   (row index)
      k_val = pos           (column index)
   a(n) = T(n_val,k_val) where T(n,k) is the k‑th positive integer whose
   binary representation contains the binary representation of n as a
   substring.  Values for n,k up to 200 are pre‑computed; larger values are
   computed on the fly if required.  */

#include <stdbool.h>
#include <stdint.h>

/* ------- helper: bit length of a 64‑bit integer ------- */
static inline unsigned bitlen_u64(uint64_t x)
{
    unsigned l = 0;
    while (x) { l++; x >>= 1; }
    return l;
}

/* ------- helper: does x contain pat (pat_len bits) as a substring? ------- */
static inline bool contains_pattern_u64(uint64_t x, uint64_t pat, unsigned pat_len)
{
    if (pat_len == 0) return true;
    unsigned x_len = bitlen_u64(x);
    if (x_len < pat_len) return false;
    uint64_t mask = ((pat_len == 64) ? ~ (uint64_t)0
                        : ((uint64_t)1 << pat_len) - 1);
    for (int shift = (int)(x_len - pat_len); shift >= 0; --shift) {
        if ( ((x >> shift) & mask) == pat ) return true;
    }
    return false;
}

/* ------- precomputation table ------- */
#define MAX_T 200
static seq_t T[MAX_T+1][MAX_T+1];
static bool built = false;

static void build_T(void)
{
    for (int n = 1; n <= MAX_T; ++n) {
        uint64_t pat = (uint64_t)n;
        unsigned pat_len = 0;
        uint64_t tmp = pat;
        while (tmp) { pat_len++; tmp >>= 1; }
        uint64_t cand = pat;      /* the first possible number is n itself */
        int k = 1;
        while (k <= MAX_T) {
            if (contains_pattern_u64(cand, pat, pat_len)) {
                T[n][k] = (seq_t)cand;
                ++k;
            }
            ++cand;
        }
    }
    built = true;
}

/* ------- fallback: compute T(n,k) directly when out of pre-computed range ------- */
static seq_t compute_T_direct(idx_t n, idx_t k)
{
    uint64_t pat = (uint64_t)n;
    unsigned pat_len = 0;
    uint64_t tmp = pat;
    while (tmp) { pat_len++; tmp >>= 1; }
    uint64_t cand = n;               /* start at n itself */
    idx_t found = 0;
    while (found < k) {
        if (contains_pattern_u64(cand, pat, pat_len))
            ++found;
        if (found == k) return (seq_t)cand;
        ++cand;
    }
    return (seq_t)0; /* unreachable */
}

/* ------- main sequence function ------- */
seq_t a(idx_t n)
{
    if (!built) build_T();

    /* find the diagonal number t such that t*(t+1)/2 ≥ n */
    idx_t t = 1;
    while (((t + 1) * t) / 2 < n) ++t;

    idx_t prev = ((t - 1) * t) / 2;          /* elements before this diagonal */
    idx_t pos  = n - prev;                   /* 1‑based position inside the diagonal */

    idx_t n_val = t + 1 - pos;               /* row index (n in T)   */
    idx_t k_val = pos;                       /* column index (k in T) */

    if (n_val <= MAX_T && k_val <= MAX_T)
        return T[(int)n_val][(int)k_val];

    /* if the requested indices are beyond the pre‑computed range,
       compute the value on the fly (this case should not occur for
       the typical test limits). */
    return compute_T_direct(n_val, k_val);
}
