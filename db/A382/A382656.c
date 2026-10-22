/*
 * A382656  a(n) = L(2*n+1)+4*n+2.
 * https://oeis.org/A382656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A382656
 *
 * Formula: a(n) = L(2*n+1) + 4*n + 2
 *   where L(k) are the Lucas numbers:
 *     L(0) = 2, L(1) = 1, L(k) = L(k-1) + L(k-2) for k >= 2
 *
 * The implementation evaluates the required Lucas number iteratively.
 * It works for any seq_t supporting + and *, and for any idx_t.
 * Complexity: O(n) per call, fast enough for n up to ~10 000.
 */
seq_t a(idx_t n)
{
    /* Compute the index of the Lucas number that is needed. */
    idx_t k = 2 * n + 1;

    /* Base Lucas values. */
    seq_t l_prev = (seq_t)2; /* L(0) */
    seq_t l_curr = (seq_t)1; /* L(1) */

    seq_t lucas_value;

    if (k == 0) {
        lucas_value = l_prev;
    } else if (k == 1) {
        lucas_value = l_curr;
    } else {
        /* Iteratively compute L(k) for k >= 2. */
        for (idx_t i = 2; i <= k; ++i) {
            seq_t l_next = l_prev + l_curr;
            l_prev = l_curr;
            l_curr = l_next;
        }
        lucas_value = l_curr;
    }

    /* a(n) = L(2*n+1) + 4*n + 2. */
    seq_t four_n = (seq_t)4 * (seq_t)n;
    seq_t result = lucas_value + four_n + (seq_t)2;
    return result;
}
