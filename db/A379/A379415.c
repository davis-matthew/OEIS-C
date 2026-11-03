/*
 * A379415  a(n) = n + floor(n*r/s) + floor(n*t/s), where r = 3^(1/4), s =
 *          3^(1/2), t = 3^(3/4).
 * https://oeis.org/A379415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/r) + floor(n*r), where r = 3^(1/4)
 * Range:     u8 none, u16 n <= 3, u32 n <= 56, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor(n * r^k) for k = 1 (r) or k = 3 (s = r^3 = 3^(3/4)).
   The algorithm uses integer comparisons on fourth powers:
   floor(n * r)  <=>  find maximal k such that k^4 ≤ n^4 * 3,
   floor(n * s)  <=>  find maximal k such that k^4 ≤ n^4 * 27. */
static seq_t floor_n_root_factor(idx_t n, int factor)
{
    /* Use the 64‑bit index type for intermediate powers. */
    idx_t N   = n;
    idx_t n4  = N * N * N * N;          /* n^4 (fits in uint64_t for n ≤ 10 000) */
    idx_t target64 = n4 * (idx_t)factor; /* n^4 * factor */

    /* Convert to seq_t for comparisons and further arithmetic. */
    seq_t target = (seq_t)target64;

    seq_t low  = 0;
    seq_t high = 1;
    /* Find an upper bound: the smallest power of two whose fourth power
       exceeds the target. */
    while (1) {
        seq_t h4 = high * high;
        h4 = h4 * h4;   /* h^4 */
        if (h4 > target) break;
        high <<= 1;
    }

    /* Binary search for the largest k with k^4 ≤ target. */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t m4  = mid * mid;
        m4 = m4 * m4;   /* mid^4 */
        if (m4 <= target)
            low = mid;
        else
            high = mid - 1;
    }
    return low;      /* floor(n * r^k) */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safeguard for unsafe calls */

    seq_t N     = (seq_t)n;

    /* floor(n * r)  →  factor 3  (since r^4 = 3) */
    seq_t floor_nr = floor_n_root_factor(n, 3);

    /* floor(n * r^3) → factor 27 (since (r^3)^4 = 27) */
    seq_t floor_ns = floor_n_root_factor(n, 27);

    /* floor(n / r) = floor( floor(n * r^3) / 3 ) */
    seq_t floor_nr_inv = floor_ns / 3;

    return N + floor_nr + floor_nr_inv;
}
