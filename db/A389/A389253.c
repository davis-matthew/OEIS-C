/*
 * A389253  Expansion of e.g.f. (1/3) * (exp((1 + x)^3 - 1) + 2).
 * https://oeis.org/A389253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A389253 – Expansion of e.g.f. (1/3)*(exp((1+x)^3-1)+2).  
   Recurrence (offset 0):  
     a(0)=1, a(1)=1, a(2)=5, a(3)=29.  
     For n ≥ 4:  
         a(n) = 3*a(n-1) + 6*(n-1)*a(n-2) + 3*(n-1)*(n-2)*a(n-3). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)29;

    seq_t prev2 = (seq_t)1;   // a_{n-3} for the first iteration (n=4)
    seq_t prev1 = (seq_t)5;   // a_{n-2}
    seq_t cur   = (seq_t)29;  // a_{n-1}

    for (idx_t k = 4; k <= n; ++k)
    {
        seq_t term1 = cur       * (seq_t)3;
        seq_t term2 = prev1     * (seq_t)6 * (seq_t)(k - 1);
        seq_t term3 = prev2     * (seq_t)3 * (seq_t)(k - 1) * (seq_t)(k - 2);
        seq_t next  = term1 + term2 + term3;

        prev2 = prev1;
        prev1 = cur;
        cur   = next;
    }
    return cur;
}
