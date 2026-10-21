/*
 * A272921  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 555", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272921: a(n) = 3*4^n - 4*3^(n-2) - 4*2^n + 1   (n>1)
 * a(0) = 1, a(1) = 5
 */

static seq_t pow_uint(idx_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1) result *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t t1 = (seq_t)3 * pow_uint(4, n);          /* 3 * 4^n      */
    seq_t t2 = (seq_t)4 * pow_uint(3, n - 2);      /* 4 * 3^(n-2)   */
    seq_t t3 = (seq_t)4 * pow_uint(2, n);          /* 4 * 2^n       */

    seq_t res = t1;
    res -= t2;
    res -= t3;
    res += (seq_t)1;
    return res;
}
