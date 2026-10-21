/*
 * A285917  Number of ordered set partitions of [n] into two blocks such that
 *          equal-sized blocks are ordered with increasing least elements.
 * https://oeis.org/A285917
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=1}^{⌊(n-1)/2⌋} C(n,k) + [n even] * C(n,n/2)/2
 *            This counts ordered set partitions of [n] into two blocks with
 *            the required ordering rule. The implementation uses only integer
 *            arithmetic, with a 128‑bit intermediate for 64‑bit builds
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* n < offset: undefined */

    const idx_t m = (n - 1) / 2;              /* largest k < n/2 */

    /* 64‑bit builds: use __int128 intermediates to avoid overflow */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum      = 0;       /* Σ C(n,k) for k=1..m   */
        unsigned __int128 binom    = 1;       /* current C(n,k)        */

        for (idx_t k = 1; k <= m; ++k) {
            binom = binom * (unsigned __int128)(n - k + 1) / k;
            sum += binom;
        }

        unsigned __int128 result = sum * 2;   /* contribute unequal sizes */

        if ((n & 1U) == 0U) {                /* n even: add equal‑size term */
            unsigned __int128 binom_half =
                binom * (unsigned __int128)(n - m) / (m + 1); /* C(n,n/2) */
            result += binom_half / 2;
        }

        return (seq_t)result;                /* cast back to seq_t         */
    }

    /* Arbitrary‑precision builds: use seq_t arithmetic only */
    seq_t sum   = 0;
    seq_t binom = 1;

    for (idx_t k = 1; k <= m; ++k) {
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
        sum   = sum + binom;
    }

    seq_t result = sum * 2;                  /* unequal‑size blocks       */

    if ((n & 1U) == 0U) {                    /* n even: add equal size    */
        seq_t binom_half = binom * (seq_t)(n - m) / (seq_t)(m + 1); /* C(n,n/2) */
        result = result + binom_half / 2;
    }

    return result;
}
