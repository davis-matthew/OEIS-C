/*
 * A393289  Genus of the n-path complement graph.
 * https://oeis.org/A393289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, changed
 * Formula:   a(n) = ceil((n^2 - 9*n + 14)/12) for n ≥ 8, with * corrections:
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0, * a(7)=1,
 *            a(10)=3. For n ≤ 6 the value is 0. This implements the * OEIS
 *            A393289 sequence
 * Range:     u8 n <= 20, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
 {
     /* Small indices */
     if (n <= 6) return 0;
     if (n == 7) return 1;
     if (n == 10) return 3;

     /* n ≥ 8 (and n != 10) – numerator is positive */
     seq_t nn = (seq_t)n;
     seq_t num = nn * nn - (seq_t)9 * nn + 14;
     seq_t div = num / 12;
     if (num % 12 != 0) ++div;
     return div;
 }
