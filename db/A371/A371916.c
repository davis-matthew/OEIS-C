/*
 * A371916  Zeroless analog of tetranacci numbers.
 * https://oeis.org/A371916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Zr(a(n-1)+a(n-2)+a(n-3)+a(n-4)) with initial values
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1. Zr removes all decimal zero
 *            digits from its argument
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t remove_zero_digits(seq_t n)
{
    /* recursively remove zero digits from n.
       Works for any integer type that supports /, %, +, *, ==. */
    if (n == 0) return (seq_t)0;          /* base case */
    seq_t q = n / 10;                     /* higher digits */
    seq_t r = n % 10;                     /* lowest digit (0–9) */
    seq_t higher = remove_zero_digits(q); /* processed higher part */
    if (r == 0) return higher;            /* skip zero digit */
    return higher * 10 + r;               /* append non‑zero digit */
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)1;           /* a(0) … a(3) are 1 */

    seq_t t0 = 1, t1 = 1, t2 = 1, t3 = 1; /* sliding window of last four values */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t sum  = t0 + t1 + t2 + t3;
        seq_t next = remove_zero_digits(sum);
        t0 = t1;   /* shift the window */
        t1 = t2;
        t2 = t3;
        t3 = next;
    }

    return t3; /* a(n) stored in t3 after loop */
}
