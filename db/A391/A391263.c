/*
 * A391263  The number of two-component spanning forests of the wheel graph
 *          W_{n+1} in which vertices v_1 and v_3 lie in different components.
 * https://oeis.org/A391263
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2 * (Lucas(2*n-2) - 3)
 *
 * Uses the Lucas recurrence:
 *   L_0 = 2
 *   L_1 = 1
 *   L_k = L_{k-1} + L_{k-2}  for k >= 2
 *
 * Since only additions and a single subtraction/multiplication are needed,
 * this works for any seq_t (uint8_t, uint16_t, uint32_t, uint64_t,
 * unsigned __int128, or arbitrary‑precision type) and retains
 * exactness for all values that fit in seq_t.
 */

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset; return 0. */
    if (n < 2)
        return (seq_t)0;

    /* k = 2*n - 2, the index of the needed Lucas number. */
    idx_t k = 2 * n - 2;

    /* Handle small k specially to avoid an empty loop. */
    seq_t l_prev_prev = (seq_t)2;   /* L_0 */
    seq_t l_prev      = (seq_t)1;   /* L_1 */
    seq_t l_curr = l_prev_prev;     /* will hold L_k */

    if (k == 0) {
        l_curr = l_prev_prev;      /* L_0 */
    } else if (k == 1) {
        l_curr = l_prev;           /* L_1 */
    } else {
        /* Compute Lucas numbers up to L_k. */
        for (idx_t i = 2; i <= k; ++i) {
            l_curr = l_prev_prev + l_prev;   /* L_i = L_{i-2} + L_{i-1} */
            l_prev_prev = l_prev;
            l_prev = l_curr;
        }
    }

    /* a(n) = 2 * (L_k - 3) */
    seq_t result = (l_curr - (seq_t)3) * (seq_t)2;
    return result;
}
