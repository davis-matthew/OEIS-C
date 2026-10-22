/*
 * A320989  Fibonacci rabbit sequence number n coded in base seven.
 * https://oeis.org/A320989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0)=0, a(1)=1, a(2)=7;
    For n≥3:  
        a(n) = a(n-1) * 7^{len(n-2)} + a(n-2),
    where len(k) is the length of the base‑7 representation of a(k),
    satisfying len(1)=1, len(2)=2 and len(k)=len(k-1)+len(k-2).
    This is the Fibonacci word “10, 101, 10110, …” interpreted in base 7. */
seq_t a(idx_t n)
{
    /* Basic cases */
    if (n == 0U)                     return (seq_t)0;
    if (n == 1U)                     return (seq_t)1;
    if (n == 2U)                     return (seq_t)7;

    /* Values for indices 1 and 2   (a(1)=1, a(2)=7) */
    seq_t valPrevPrev = (seq_t)1;     /* a(1)     */
    seq_t valPrev     = (seq_t)7;     /* a(2)     */

    /* Powers of 7 for the lengths of a(1) and a(2)   (7¹ = 7, 7² = 49) */
    seq_t powPrevPrev = (seq_t)7;      /* 7^{len(1)} = 7¹     */
    seq_t powPrev     = (seq_t)49;     /* 7^{len(2)} = 7²     */

    /* Iterate from index 3 up to the requested n */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* a(i) = a(i-1) * 7^{len(i-2)} + a(i-2) */
        seq_t valCurr = valPrev * powPrevPrev + valPrevPrev;

        /* 7^{len(i)} = 7^{len(i-1)} * 7^{len(i-2)} */
        seq_t powCurr = powPrev * powPrevPrev;

        /* Shift for next iteration */
        valPrevPrev = valPrev;
        valPrev     = valCurr;
        powPrevPrev = powPrev;
        powPrev     = powCurr;
    }

    return valPrev;
}
