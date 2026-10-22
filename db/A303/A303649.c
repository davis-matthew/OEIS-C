/*
 * A303649  Number of involutions of [n] having exactly one peak.
 * https://oeis.org/A303649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 4*a(n-3) + 10*a(n-4) - 4*a(n-5) -
 *            4*a(n-6) + 4*a(n-7) - a(n-8) (for n >= 8)
 * Range:     u8 n <= 8, u16 n <= 27, u32 n <= 252, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms a(0) … a(7) */
    static const seq_t pre[8] = {0, 0, 0, 1, 6, 18, 44, 91};

    if (n <= 7)
        return pre[n];

    /* Rolling window of the last 8 values:
         arr[0] = a(k-1), arr[1] = a(k-2), ..., arr[7] = a(k-8) */
    seq_t arr[8] = {pre[7], pre[6], pre[5], pre[4],
                    pre[3], pre[2], pre[1], pre[0]};

    seq_t next;
    for (idx_t i = 8; i <= n; ++i)
    {
        /* Compute next term via the recurrence, avoiding negative
           intermediate values by separating positive and negative
           contributions. */
        seq_t pos = 4 * arr[0] + 10 * arr[3] + 4 * arr[6];
        seq_t neg = 4 * arr[1] + 4 * arr[2] + 4 * arr[4] + 4 * arr[5] + arr[7];
        next = pos - neg;

        /* Shift window: drop oldest value, make room for the new one */
        for (int k = 7; k > 0; --k)
            arr[k] = arr[k - 1];
        arr[0] = next;
    }
    return arr[0];
}
