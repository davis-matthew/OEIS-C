/*
 * A280724  Expansion of 1/(1 - x) + (1/(1 - x)^2)*Sum_{k>=0} x^(3^k).
 * https://oeis.org/A280724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 + n + Sum_{k=1..n} floor(log_3(k))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) gives the sum of base‑3 digit lengths of all integers 0..n */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* base case: a(0) = 1 */

    /* compute 3^L where L = floor(log_3(n)) */
    idx_t powL = 1;                 /* 3^0 */
    idx_t L    = 0;
    while (powL <= n / 3) {         /* ensure no overflow */
        powL *= 3;                  /* powL becomes 3^L after loop */
        L++;
    }

    /* sum of floor(log_3(k)) for k = 1..n */
    seq_t sum = 0;
    seq_t currPow = 1;              /* 3^0 */

    /* full intervals where floor(log_3(k)) = d for d = 0..L-1 */
    for (idx_t d = 0; d < L; ++d) {
        sum += (seq_t)d * 2 * currPow;   /* 2*3^d numbers have value d */
        currPow *= 3;                    /* next power of 3 */
    }

    /* last incomplete interval from 3^L to n */
    seq_t countLast = (seq_t)(n - powL + 1);   /* numbers with floor = L */
    sum += (seq_t)L * countLast;

    /* a(n) = 1 (for 0) + n + sum */
    return (seq_t)1 + (seq_t)n + sum;
}
