/*
 * A277561  a(n) = Sum_{k=0..n} ({binomial(n+2k,2k)*binomial(n,k)} mod 2).
 * https://oeis.org/A277561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n+2k,2k)*binomial(n,k) mod 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=1, a(1)=2; for n>1:
   if n even: a(n)=a(n/2);
   if n%4==1:  a(n)=2*a(n/4);
   if n%4==3:  a(n)=a(2*((n-3)/4)+1);
*/
seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if ((n & 1) == 0) {          // n even
        return a(n >> 1);       // n/2
    }
    if ((n & 3) == 1) {          // n ≡ 1 (mod 4)
        return 2 * a(n >> 2);   // 2 * a(n/4)
    }
    /* n ≡ 3 (mod 4) */
    idx_t m = (n - 3) >> 2;      // m = (n-3)/4
    return a((m << 1) + 1);      // a(2*m + 1)
}
