/*
 * A352513  Number of nonfixed points in the n-th composition in standard
 *          order.
 * https://oeis.org/A352513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the number of non‑fixed points in the n-th composition
 * in standard order (graded reverse‑lexicographic, A066099),
 * which is defined for OEIS A352513.
 *
 * The algorithm follows the OEIS construction:
 *   - Let k = n.
 *   - The set of positions (1‑based, numbered from the least significant
 *     bit upwards) where k has a 1-bit gives a list p₁ < p₂ < … < p_m.
 *   - Prepend a 0 and take consecutive differences: d_i = p_i − p_{i-1}
 *     with p₀ = 0.
 *   - Reverse the differences to obtain the composition parts c₁, …, c_m.
 *   - The answer is the number of indices i such that c_i ≠ i.
 *
 * Complexity:  O(log k) per call, well within the limits for n ≤ 10,000.
 */
seq_t a(idx_t n)
{
    /* Find 1‑bit positions in increasing order (least significant first) */
    unsigned int pos[64];
    unsigned int m = 0;
    unsigned int bit_idx = 0;
    idx_t temp = n;
    while (temp) {
        if (temp & 1)
            pos[m++] = bit_idx + 1;   /* 1‑based numbering */
        temp >>= 1;
        bit_idx++;
    }

    if (m == 0)               /* n == 0  ⇒ empty composition */
        return (seq_t)0;

    /* Compute first differences d₁ … d_m */
    unsigned int diff[64];
    for (unsigned int j = 0; j < m; ++j) {
        diff[j] = (j == 0) ? pos[0] : pos[j] - pos[j - 1];
    }

    /* Count non‑fixed points by examining the reversed differences */
    unsigned int count = 0;
    for (unsigned int j = 0; j < m; ++j) {
        unsigned int part = diff[m - 1 - j];     /* reversed order */
        if (part != j + 1)                      /* 1‑based index in composition */
            ++count;
    }

    return (seq_t)count;
}
