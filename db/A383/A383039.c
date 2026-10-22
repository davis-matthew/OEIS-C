/*
 * A383039  Sum of the legs of the unique primitive Pythagorean triple (a,b,c)
 *          such that (a-b+c)/2 = A000032(n) and its long leg and hypotenuse
 *          are consecutive natural numbers.
 * https://oeis.org/A383039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Lucas(n)^2 - 1 𝐿(0)=2, 𝐿(1)=1, 𝐿(n)=𝐿(n-1)+𝐿(n-2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute Lucas(n) iteratively */
seq_t a(idx_t n)
{
    /* special cases that also satisfy the recurrence */
    if (n == 0u) return (seq_t)7;  /* 2*2^2 - 1 = 7 */
    if (n == 1u) return (seq_t)1;  /* 2*1^2 - 1 = 1 */

    /* iterate to n (n ≥ 2) */
    seq_t prev2 = (seq_t)2;   /* L(0) */
    seq_t prev1 = (seq_t)1;   /* L(1) */
    seq_t curr;

    for (idx_t i = 2u; i <= n; ++i) {
        curr = prev1 + prev2;  /* L(i) = L(i-1)+L(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }

    /* curr now holds L(n) */
    seq_t lucas = curr;
    seq_t lucas_sq = lucas * lucas;   /* may overflow in fixed-size builds,
                                         but that is acceptable for values
                                         that fit in seq_t */
    return (seq_t)( (lucas_sq << 1) - (seq_t)1 );  /* 2*lucas^2 - 1 */
}
