/*
 * A260444  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (1,0,0) and midword sequence (a(n)); see A260390.
 * https://oeis.org/A260444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = a(3) = 0. For n > 3 let k be the unique integer
 *            with 2^k ≤ n ≤ 2^{k+1} – 1. Then if n = 2^k a(n) = a(k-1)
 *            (mid‑word step) else a(n) = a(2^{k+1} – n) (reflection in a
 *            palindrome) Indices start at 1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2 || n == 3) return (seq_t)0;

    while (n > 3) {
        /* find the exponent k such that 2^(k+1) > n ≥ 2^k */
        idx_t k = 1;
        /* while 2^(k+1) ≤ n, increase k */
        while (((idx_t)1 << (k + 1)) <= n) ++k;

        idx_t center = ((idx_t)1 << k);      /* 2^k, the palindrome centre */
        if (n == center) {                   /* mid‑word case */
            n = k - 1;                       /* a(2^k) = a(k-1) */
        } else {                             /* reflection case */
            n = (idx_t)2 * center - n;       /* mirror across the centre */
        }
    }

    /* after the loop n ∈ {1,2,3} */
    return (n == 1) ? (seq_t)1 : (seq_t)0;
}
