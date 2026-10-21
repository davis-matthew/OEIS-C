/*
 * A259600  Triangular array: sums of two distinct lower Wythoff numbers.
 * https://oeis.org/A259600
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the (n‑th) entry of the triangular array whose
 * row r (r ≥ 2) contains the values u(m)+u(r) for m = 1 … r–1,
 * where u(k) = ⌊k·φ⌋ is the lower Wythoff sequence
 * (φ = (1+√5)/2).  The function computes this entry for any n ≥ 2.
 * Intrinsic integer arithmetic is used; no floating point is employed.
 */

#include <stdint.h>

static const uint64_t PHI_NUM = 161803398874989ULL;   /* ≈ φ × 10^14 */
static const uint64_t PHI_DEN = 100000000000000ULL;   /* 10^14          */

/* Integer floor multiplication: ⌊k·φ⌋  */
static inline seq_t floor_k_phi(idx_t k)
{
    /* Multiply with 128‑bit to avoid overflow, then divide. */
    return (seq_t)(((__int128)k * PHI_NUM) / PHI_DEN);
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* below the offset – value irrelevant */

    /* Number of entries before the current row (index shifted by –1): */
    idx_t target = n - 1;          /* 1‑based position in the flattened array */

    /* Find the row r such that (r‑1)*r/2 ≥ target (r ≥ 2). */
    idx_t low  = 2;
    idx_t high = 2;
    while ((high - 1) * high / 2 < target)
        high <<= 1;                /* exponential search to bound high */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid - 1) * mid / 2 >= target)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of elements before row r (0‑based). */
    idx_t before = (r - 2) * (r - 1) / 2;

    /* Position inside the row (0‑based). */
    idx_t offset = target - before - 1;

    /* m ranges from 1 to r−1. */
    idx_t m = offset + 1;

    /* Compute the two lower Wythoff numbers and return their sum. */
    seq_t um = floor_k_phi(m);
    seq_t ur = floor_k_phi(r);
    return um + ur;
}
