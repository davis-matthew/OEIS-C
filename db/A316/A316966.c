/*
 * A316966  Same as A316671, except numbering of the squares starts at 0
 *          rather than 1.
 * https://oeis.org/A316966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (n + 2)*(n - (-1)^n)/2
 * Range:     u8 n <= 22, u16 n <= 360, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalent: a(0)=0; for n>0, if n even: a(n)=1+((n+2)*(n-1))/2,
                 if n odd:  a(n)=1+((n+2)*(n+1))/2) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    long long nn = (long long)n;
    long long val;
    if ((n & 1ULL) == 0) {          /* n even */
        val = 1 + (nn + 2) * (nn - 1) / 2;
    } else {                        /* n odd */
        val = 1 + (nn + 2) * (nn + 1) / 2;
    }
    return (seq_t)val;
}
