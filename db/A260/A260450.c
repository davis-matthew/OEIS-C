/*
 * A260450  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (1,3,2) and midword sequence (a(n)); see Comments.
 * https://oeis.org/A260450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) defined by w(1) = (1,3,2) and for n ≥ 2
   w(n) = w(n–1) + a(n–1) + reverse(w(n–1)).
   The function below implements a(n) by a simple recursive mapping
   that follows from the palindrome property. */
seq_t a(idx_t n)
{
    /* Base cases */
    static const seq_t base_vals[4] = {0, (seq_t)1, (seq_t)3, (seq_t)2};
    if (n <= 3) return base_vals[n];

    while (n > 3) {
        /* Find the largest power of two ≤ n: 2^k = bit. */
        idx_t bit = 1;
        int k = 0;
        while ((bit << 1) <= n) {
            bit <<= 1;
            ++k;
        }

        if (n == bit) {
            /* n is the center of a palindrome: a(2^k) = a(k‑1). */
            n = (idx_t)(k - 1);
        } else {
            /* n lies in the right half: mirror it to the left. */
            if (k == 63) {
                /* 2^(k+1) would overflow uint64_t; use modulo arithmetic. */
                n = ((idx_t)~0) - n + 1;      /* n = 2^64 – n */
            } else {
                n = (bit << 1) - n;           /* n = 2^(k+1) – n */
            }
        }
    }
    return base_vals[n];
}
