/*
 * A390700  a(n) = Sum_{k=0..n} (k+1) * 2^k * binomial(2*k,2*(n-k)).
 * https://oeis.org/A390700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A390700
   a(n) = Sum_{k=0..n} (k+1) * 2^k * binomial(2*k, 2*(n-k))
   Recurrence: a(n) = 8*a(n-1) - 16*a(n-2) + 16*a(n-3) - 72*a(n-4)
                 + 32*a(n-5) - 64*a(n-6) + 64*a(n-7) - 16*a(n-8)
   Initial values: a(0..7) = {1,4,16,104,572,2912,14720,73152}
*/
seq_t a(idx_t n)
{
    static const seq_t init[8] = {1,4,16,104,572,2912,14720,73152};

    if (n < 8u) return init[n];

    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i) buf[i] = init[i];

    seq_t result = 0;
    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t a1 = buf[(i-1)&7];
        seq_t a2 = buf[(i-2)&7];
        seq_t a3 = buf[(i-3)&7];
        seq_t a4 = buf[(i-4)&7];
        seq_t a5 = buf[(i-5)&7];
        seq_t a6 = buf[(i-6)&7];
        seq_t a7 = buf[(i-7)&7];
        seq_t a8 = buf[(i-8)&7];

        seq_t tmp_pos = 8 * a1 + 16 * a3 + 32 * a5 + 64 * a7;
        seq_t tmp_neg = 16 * a2 + 72 * a4 + 64 * a6 + 16 * a8;

        result = tmp_pos - tmp_neg;   // guaranteed non‑negative for all n
        buf[i & 7] = result;
    }
    return result;
}
