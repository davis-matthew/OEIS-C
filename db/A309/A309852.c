/*
 * A309852  Array read by antidiagonals: ((z+sqrt(x))/2)^k + ((z-sqrt(x))/2)^k
 *          for columns k >= 0 and rows n >= 0, where x = 4*n+1 and y =
 *          floor(sqrt(x)) and z = y-1+(y mod 2).
 * https://oeis.org/A309852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A309852: array read by antidiagonals.
    For each row r (r ≥ 0) set
        x = 4*r + 1,
        y = ⌊√x⌋,
        z = y – 1 + (y mod 2).
    Define T(r,0)=2, T(r,1)=z and
        T(r,k)=T(r,k‑1)*z + T(r,k‑2)*((x – z²)/4)   for k ≥ 2.
    a(n) is obtained by reading the triangle T along anti‑diagonals
    (rows i, columns j with i+j = constant).

    The implementation below follows this definition exactly,
    without using any floating‑point operations. */

#include <stdint.h>

/* Integer square root: floor(sqrt(v)) for 0 ≤ v ≤ UINT64_MAX. */
static inline uint64_t floor_sqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= v) {
            ans = mid;
            lo  = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Small‑int multiplication that uses a 128‑bit intermediate when
   seq_t is at most 64 bits wide and the compiler supports __int128.
   For larger seq_t types (e.g. arbitrary‑precision) we simply
   rely on the type's own multiplication operator. */
static inline seq_t mul_small(seq_t a, seq_t b)
{
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) <= 8) {
        return (seq_t)(((__int128)a) * b);
    }
#endif
    return a * b;
}

seq_t a(idx_t n)
{
    /* -------- Find the anti‑diagonal containing position n -------- */
    /* Compute d = ⌊(√(8n+1) – 1)/2⌋ using integer arithmetic. */
    uint64_t tmp = (uint64_t)n * 8 + 1;          /* 8n+1 fits in 64 bits for n ≤ 10⁴ */
    uint64_t sqrt8 = floor_sqrt_u64(tmp);
    uint64_t d = (sqrt8 - 1) >> 1;               /* diagonal index */

    /* Index of the first element on diagonal d */
    idx_t start = (idx_t)((__int128)d * (d + 1) / 2);

    /* Row and column within that diagonal */
    idx_t row = n - start;                       /* r */
    idx_t col = d - row;                         /* k */

    /* -------- Compute T(row, col) from its recurrence -------- */
    uint64_t x = 4ULL * row + 1ULL;               /* x = 4*r + 1 */
    uint64_t y = floor_sqrt_u64(x);              /* y = ⌊√x⌋ */
    uint64_t z = y - 1 + (y & 1ULL);             /* z = y – 1 + (y mod 2) */

    seq_t t1 = (seq_t)z;                         /* T(row,1) */
    uint64_t z_sq = z * z;
    seq_t c   = (seq_t)((x - z_sq) >> 2);        /* coefficient (x – z²)/4 */

    if (col == 0) return (seq_t)2;
    if (col == 1) return t1;

    seq_t prev2 = (seq_t)2;                      /* T(row,0) */
    seq_t prev1 = t1;                            /* T(row,1) */

    for (idx_t i = 2; i <= col; ++i) {
        seq_t prod1 = mul_small(prev1, t1);
        seq_t prod2 = mul_small(prev2, c);
        seq_t curr  = prod1 + prod2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
