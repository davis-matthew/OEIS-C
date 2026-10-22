/*
 * A390924  Expansion of 1/((1-2*x) * sqrt(1-12*x)).
 * https://oeis.org/A390924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/****************************************************
 * OEIS A390924: Expansion of 1/((1-2*x) * sqrt(1-12*x))
 *
 * Formula (explicit sum):
 *   a(n) = Σ_{k=0..n} 3^k * 2^(n-k) * binomial(2*k,k)
 *
 * Recurrence (used for efficient computation):
 *   n*a(n) = (14n-6)*a(n-1) - 12*(2n-1)*a(n-2)
 *   a(0) = 1, a(1) = 8
 *
 * This implementation chooses a 128‑bit intermediate for
 * small integer types (≤ 64‑bit) to avoid overflow while
 * keeping all arithmetic exact. For larger seq_t types
 * (e.g. 128‑bit or arbitrary‑precision) the normal
 * seq_t operations are used.
 ****************************************************/

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    /* Small (≤ 64‑bit) seq_t: use 128‑bit intermediates */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        seq_t prev2 = (seq_t)1;   /* a(0) */
        seq_t prev1 = (seq_t)8;   /* a(1) */
        seq_t cur = 0;
        for (idx_t i = 2; i <= n; ++i) {
            unsigned __int128 t1 = (unsigned __int128)(14*i - 6)
                                  * (unsigned __int128)prev1;
            unsigned __int128 t2 = (unsigned __int128)12
                                  * (unsigned __int128)(2*i - 1)
                                  * (unsigned __int128)prev2;
            cur = (seq_t)((t1 - t2) / (unsigned __int128)i);
            prev2 = prev1;
            prev1 = cur;
        }
        return cur;
    }

    /* Generic seq_t (≥ 128‑bit or arbitrary‑precision)
       use direct seq_t arithmetic – no overflow concern   */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = (seq_t)(14*i - 6) * prev1;
        seq_t term2 = (seq_t)12 * (seq_t)(2*i - 1) * prev2;
        cur = (term1 - term2) / (seq_t)i;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
