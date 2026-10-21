/*
 * A245487  Number of compositions of n into parts 3,4 where both parts are
 *          always present.
 * https://oeis.org/A245487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)+b(n) for n≥7, a(n)=0 for n<7. b(n) is the
 *            12‑cycle (1,0,1,0,1,1,0,0,2,0,0,1) with b(n) = cycle[(n+1) mod
 *            12]
 * Range:     u8 n <= 33, u16 n <= 61, u32 n <= 117, u64 n <= 228,
 *            u128 n <= 451, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 7 the answer is 0 – no compositions exist. */
    if (n < 7) return (seq_t)0;

    /* Small sliding window that holds a(k) at position k mod 8. */
    seq_t buf[8];
    for (int j = 0; j < 8; ++j) buf[j] = 0;

    /* 12‑cycle for b(n). */
    static const unsigned char cycle[12] = {
        1, 0, 1, 0, 1, 1, 0, 0, 2, 0, 0, 1
    };

    for (idx_t i = 7; i <= n; ++i) {
        int b_idx = (int)((i + 1) % 12);
        seq_t bi = (seq_t)cycle[b_idx];
        seq_t next = buf[(i - 3) % 8] + buf[(i - 4) % 8] + bi;
        buf[i % 8] = next;
    }

    return buf[n % 8];
}
