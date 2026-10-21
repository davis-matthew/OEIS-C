/*
 * A291587  a(n) = ((2n-1)!!)^5 * Sum_{i=1..n} 1/(2*i-1)^5.
 * https://oeis.org/A291587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n-1)!!)^5 * Sum_{i=1..n} 1/(2i-1)^5 Recurrence:
 *            a(0)=0, a(1)=1. a(n+1)=((2n-1)^5+(2n+1)^5)*a(n) -
 *            (2n-1)^10*a(n-1), for n>0
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow5(seq_t x) {
    seq_t x2 = x * x;        // x^2
    seq_t x4 = x2 * x2;      // x^4
    return x4 * x;           // x^5
}

static inline seq_t pow10(seq_t x) {
    seq_t p5 = pow5(x);      // x^5
    return p5 * p5;          // x^10
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_prev2 = 0;   // a(0)
    seq_t a_prev1 = 1;   // a(1)

    for (idx_t i = 1; i < n; ++i) {
        seq_t odd = (seq_t)(2*i - 1);      // 2i-1
        seq_t odd_plus = odd + 2;          // 2i+1

        seq_t p5_minus = pow5(odd);
        seq_t p5_plus  = pow5(odd_plus);

        seq_t sum5 = p5_minus + p5_plus;
        seq_t p10_minus = pow10(odd);

        seq_t temp = sum5 * a_prev1;
        seq_t sub  = p10_minus * a_prev2;

        seq_t a_next = temp - sub;

        a_prev2 = a_prev1;
        a_prev1 = a_next;
    }

    return a_prev1;
}
