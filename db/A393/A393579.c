/*
 * A393579  a(n) = floor(g*n) mod 3, where g = (1+sqrt(5))/2 is the golden
 *          ratio.
 * https://oeis.org/A393579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1+sqrt(5))/2*n) mod 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor_sqrt for an unsigned __int128 value using binary search. */
static unsigned __int128 isqrt128(unsigned __int128 x) {
    unsigned __int128 low = 0;
    unsigned __int128 high = 1;
    while (high * high <= x) {
        high <<= 1;
    }
    while (high - low > 1) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    /* The sequence is only defined for n >= 1; values for n == 0 are unused. */
    if (n == 0) return 0;

    /* Work in 128‑bit integers to avoid overflow for large n. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 prod = 5 * nn * nn;          /* 5*n^2  */
    unsigned __int128 sqrt = isqrt128(prod);        /* floor(sqrt(5*n^2)) = floor(n*sqrt5) */

    /* floor(n/phi) = floor((sqrt - n)/2)  */
    unsigned __int128 n_over_phi = (sqrt - nn) / 2;

    /* Compute modulo 3 only. */
    seq_t nn_mod3 = (seq_t)(nn % 3);
    seq_t nphi_mod3 = (seq_t)(n_over_phi % 3);

    return (nn_mod3 + nphi_mod3) % 3;
}
