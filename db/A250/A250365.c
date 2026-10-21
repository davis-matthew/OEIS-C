/*
 * A250365  Number of length 7 arrays x(i), i=1..7 with x(i) in i..i+n and no
 *          value appearing more than 3 times.
 * https://oeis.org/A250365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 21*n^5 + 147*n^3 + 49*n^2 + 7*n (for n>4)
 *            Base cases: a(1)=128, a(2)=2187, a(3)=16128, a(4)=76300 Computes
 *            the number of length‑7 arrays with values in i..i+n and at most
 *            three equal entries. For n>4 the analytic polynomial holds
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
     /* Base values for n = 1..4 */
     static const seq_t base[4] = {128, 2187, 16128, 76300};
     if (n <= 4) return base[n-1];

     /* Compute polynomial for n > 4 */
     seq_t nn   = (seq_t)n;      /* n */
     seq_t n2   = nn * nn;       /* n^2 */
     seq_t n3   = n2 * nn;       /* n^3 */
     seq_t n5   = n2 * n3;       /* n^5 */
     seq_t n6   = n3 * n3;       /* n^6 */
     seq_t n7   = n6 * nn;       /* n^7 */

     seq_t term1 = n7;
     seq_t term2 = (seq_t)7 * n6;
     seq_t term3 = (seq_t)21 * n5;
     seq_t term4 = (seq_t)147 * n3;
     seq_t term5 = (seq_t)49 * n2;
     seq_t term6 = (seq_t)7 * nn;

     return term1 + term2 + term3 + term4 + term5 + term6;
 }
