/*
 * A250376  Number of length 3+6 0..n arrays with every seven consecutive
 *          terms having the maximum of some three terms equal to the minimum
 *          of the remaining four terms.
 * https://oeis.org/A250376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the order‑9 linear recurrence * a(n) =
 *            9*a(n‑1) – 36*a(n‑2) + 84*a(n‑3) – 126*a(n‑4) * + 126*a(n‑5) –
 *            84*a(n‑6) + 36*a(n‑7) – 9*a(n‑8) + a(n‑9) * for n > 9, * with
 *            initial values * a(1) = 252, a(2) = 7168, a(3) = 69984, * a(4) =
 *            396745, a(5) = 1616636, a(6) = 5272892, * a(7) =...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial sequence values 1..9 */
    static const seq_t init[9] = {
        (seq_t)252,
        (seq_t)7168,
        (seq_t)69984,
        (seq_t)396745,
        (seq_t)1616636,
        (seq_t)5272892,
        (seq_t)14647808,
        (seq_t)36038649,
        (seq_t)80620540
    };

    if (n <= 9) {
        return init[n - 1];
    }

    /* buffer holds the last nine terms a(k‑8)…a(k) for current k.
       Initially k = 9 -> buffer = a(1)…a(9). */
    seq_t buffer[9];
    for (int i = 0; i < 9; ++i) {
        buffer[i] = init[i];
    }

    /* iterate recurrence from k = 10 up to n */
    for (idx_t k = 10; k <= n; ++k) {
        seq_t next_term =
            (seq_t)9   * buffer[8]   /* a(k‑1)   */
          - (seq_t)36  * buffer[7]   /* a(k‑2)   */
          + (seq_t)84  * buffer[6]   /* a(k‑3)   */
          - (seq_t)126 * buffer[5]   /* a(k‑4)   */
          + (seq_t)126 * buffer[4]   /* a(k‑5)   */
          - (seq_t)84  * buffer[3]   /* a(k‑6)   */
          + (seq_t)36  * buffer[2]   /* a(k‑7)   */
          - (seq_t)9   * buffer[1]   /* a(k‑8)   */
          +          buffer[0];     /* a(k‑9)   */

        /* shift buffer left by one to drop the oldest term */
        for (int i = 0; i < 8; ++i) {
            buffer[i] = buffer[i + 1];
        }
        buffer[8] = next_term; /* newest term */
    }

    /* after loop k == n, buffer[8] holds a(n) */
    return buffer[8];
}
