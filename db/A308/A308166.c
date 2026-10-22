/*
 * A308166  Number of integer-sided triangles with perimeter n such that the
 *          smallest side divides the largest.
 * https://oeis.org/A308166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided triangles with perimeter n such
 *            that * the smallest side divides the largest. Counted by
 *            iterating over the * smallest side a and checking whether an
 *            integer k ≥ 2 exists with * n ∈ [a(2k)+1 , a(2k+1)] (for k = 1
 *            we just test n % 3 == 0)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;

    /* Equilateral triangles */
    if (n % 3 == 0)
        count++;

    /* Triangles with smallest side a (a ≤ ⌊n/3⌋) */
    for (idx_t a = 1; a <= n / 3; ++a) {
        /* k_low = max(2, ceil((n - a)/(2a))) */
        idx_t k_low = (n + a - 1) / (2 * a);   /* ceil((n - a)/(2a)) */
        if (k_low < 2)
            k_low = 2;

        /* k_high = floor((n - 1)/(2a)) */
        idx_t k_high = (n - 1) / (2 * a);

        if (k_low <= k_high)
            count++;           /* exactly one triangle per valid (a, k) */
    }

    return count;
}
