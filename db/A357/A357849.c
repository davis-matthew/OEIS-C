/*
 * A357849  Number of integer partitions (w,x,y) summing to n such that 2w =
 *          3x + 4y.
 * https://oeis.org/A357849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Count the number of integers k (k ≥ 1) satisfying 5k + 3y = n (y
 *            = (n – 5k)/3 ≥ 1) 2k ≥ y (ensures x = 2k ≥ y) This is equivalent
 *            to k_min = max(1, ceil(n/11)) k_max = floor((n-3)/5) k ≡ 2n (mod
 *            3) So a(n) = |{k ∈ [k_min, k_max] : k ≡ 2n (mod 3)}|. The
 *            function runs in O(1) time per call
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 there can be no positive solutions. */
    if (n < 3)
        return 0;

    /* Compute lower bound k_min = ceil(n/11). */
    idx_t k_min = (n + 10) / 11;          /* ceil division */
    if (k_min < 1)
        k_min = 1;

    /* Compute upper bound k_max = floor((n-3)/5). */
    idx_t k_max = (n - 3) / 5;            /* floor division */

    if (k_min > k_max)
        return 0;

    /* Congruence class r = 2n mod 3. */
    idx_t r = (2 * (n % 3)) % 3;

    /* Find the first k ≥ k_min with k mod 3 == r. */
    idx_t mod = k_min % 3;
    idx_t first;
    if (mod <= r)
        first = k_min + (r - mod);
    else
        first = k_min + (3 - (mod - r));

    if (first > k_max)
        return 0;

    /* Count numbers in arithmetic progression with step 3. */
    idx_t count = (k_max - first) / 3 + 1;

    return (seq_t)count;
}
