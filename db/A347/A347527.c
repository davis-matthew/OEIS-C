/*
 * A347527  Partial sums of A347526.
 * https://oeis.org/A347527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1, k^4 <= n} (((n - k^4)/k) + 1)
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t fourth_root(idx_t n)
{
    /* Compute floor(n^(1/4)) for n <= 2^64-1.
       65535^4 < 2^64, 65536^4 = 2^64 > max uint64_t. */
    idx_t low = 0, high = 65535;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 mid_sq = (unsigned __int128)mid * mid;
        unsigned __int128 mid_four = mid_sq * mid_sq;
        if (mid_four <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    idx_t limit = fourth_root(n);

    for (idx_t k = 1; k <= limit; ++k) {
        unsigned __int128 k_sq = (unsigned __int128)k * k;
        unsigned __int128 k4u  = k_sq * k_sq;
        seq_t k4 = (seq_t)k4u;           /* k^4, fits in seq_t because k^4 <= n */

        seq_t diff = n_seq - k4;
        seq_t q    = diff / (seq_t)k;    /* floor((n - k^4)/k) */

        sum += q + 1;
    }

    return sum;
}
