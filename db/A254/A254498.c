/*
 * A254498  Define a(1)=2 and a(2)=3. Then, if a(n-2) and a(n-1) have the same
 *          parity, a(n)=(a(n-2)+a(n-1))/2; if not, a(n)=a(n-2)/2+a(n-1) for
 *          a(n-2) even or a(n)=a(n-2)+a(n-1)/2 for a(n-1) even.
 * https://oeis.org/A254498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 77, u32 n <= 173, u64 n <= 356,
 *            u128 n <= 714, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=3.  
   If a(n‑2) and a(n‑1) have the same parity,
   a(n) = (a(n‑2)+a(n‑1))/2.
   Otherwise, a(n‑2) is even: a(n) = a(n‑2)/2 + a(n‑1),
   or a(n‑1) is even: a(n) = a(n‑2) + a(n‑1)/2. */

seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for 0, keep 0 */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)3;

    seq_t prev2 = (seq_t)2;         /* a(n‑2) */
    seq_t prev1 = (seq_t)3;         /* a(n‑1) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        if ((prev2 & (seq_t)1) == (prev1 & (seq_t)1)) {
            /* same parity: (a+b)/2 without overflow */
            curr = (prev2 >> 1) + (prev1 >> 1) + ((prev2 & (seq_t)1) & (prev1 & (seq_t)1));
        } else if ((prev2 & (seq_t)1) == 0) {    /* a(n‑2) even */
            curr = (prev2 >> 1) + prev1;
        } else {                                   /* a(n‑1) even */
            curr = prev2 + (prev1 >> 1);
        }
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
