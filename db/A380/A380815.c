/*
 * A380815  a(n) = A379343(A378684(n)).
 * https://oeis.org/A380815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = S(r-1) + P * where S(r) = r*(2*r-1) (length of first r
 *            rows) * r is the smallest integer with S(r) ≥ n * k = n - S(r-1)
 *            (1‑based position inside row r) * m = 2*r-1 * P = * if k < m : *
 *            k is odd → P = k + m * k is even → P = m - k * else : // k ≥ m *
 *            k is odd → P = k * k is even → P = 2*m -...
 * Range:     u8 n <= 233, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* find row r with S(r-1) < n <= S(r) */
    idx_t low = 1, high = 1;
    /* expand high until it covers n */
    while ((unsigned __int128)high * ((unsigned __int128)2*high - 1) < n) {
        high <<= 1;
    }
    /* binary search for minimal r */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 current = (unsigned __int128)mid * ((unsigned __int128)2*mid - 1);
        if (current >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;

    /* S(r-1) */
    idx_t r_minus = r - 1;
    idx_t S_prev = (unsigned __int128)r_minus * ((unsigned __int128)2*r_minus - 1);

    /* position within the row */
    idx_t k = n - S_prev;          /* 1 ≤ k ≤ 4r-3 */

    /* compute P */
    idx_t m = 2*r - 1;
    idx_t P;
    if (k < m) {
        if (k & 1u) {            /* k odd */
            P = k + m;
        } else {                 /* k even */
            P = m - k;
        }
    } else {                     /* k ≥ m */
        if (k & 1u) {            /* k odd */
            P = k;
        } else {                 /* k even */
            P = 2*m - k;
        }
    }

    /* final value */
    unsigned __int128 result = (unsigned __int128)S_prev + P;
    return (seq_t)result;
}
