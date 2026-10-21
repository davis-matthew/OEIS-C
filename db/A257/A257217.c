/*
 * A257217  A257213 - A003059, where A257213(n) = min{d>0 | floor(n/d) =
 *          floor(n/(d+1))}, A003059(n) = ceiling(sqrt(n)).
 * https://oeis.org/A257217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A257213(n) - ceil(sqrt(n))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257213(n) = min{ d>0 | ⌊n/d⌋ = ⌊n/(d+1)⌋ } */
/* ceil(sqrt(n)) = smallest integer ≥ sqrt(n) */

static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return low - 1;
}

static seq_t ceil_sqrt_u64(idx_t n)
{
    idx_t fs = floor_sqrt_u64(n);
    return (seq_t)(fs * fs == n ? fs : fs + 1);
}

seq_t a(idx_t n)
{
    seq_t ceil_sq = ceil_sqrt_u64(n);          /* ceil(sqrt(n)) */
    seq_t d = 1;                               /* candidate divisor */

    /* find minimal d>0 with floor(n/d) == floor(n/(d+1)) */
    for (;;) {
        seq_t q  = ((seq_t)n) / d;
        seq_t qn = ((seq_t)n) / (d + 1);
        if (q == qn) break;
        ++d;
    }

    return d - ceil_sq;                        /* a(n) = A257213(n) - ceil(sqrt(n)) */
}
