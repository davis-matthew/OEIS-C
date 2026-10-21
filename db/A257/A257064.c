/*
 * A257064  Number of length 2 1..(n+1) arrays with every leading partial sum
 *          divisible by 2 or 3.
 * https://oeis.org/A257064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-6) - 2*a(n-7) - a(n-12) + a(n-13); initial
 *            values: a(1)=2, a(2)=4, a(3)=7, a(4)=9, a(5)=16, a(6)=18,
 *            a(7)=27, a(8)=35, a(9)=45, a(10)=49, a(11)=64, a(12)=68,
 *            a(13)=84
 * Range:     u8 n <= 22, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return zero for out-of-range index 0 (offset not defined). */
    if (n == 0) return 0;

    /* Base values up to n = 13. */
    static const seq_t base[14] =
        {0, 2, 4, 7, 9, 16, 18, 27, 35, 45, 49, 64, 68, 84};

    if (n <= 13) return base[n];

    /* Ring buffer holding the last 13 computed terms. */
    seq_t buf[13];
    for (idx_t i = 1; i <= 13; ++i)
        buf[i % 13] = base[i];

    /* Compute terms 14 .. n using the recurrence. */
    for (idx_t i = 14; i <= n; ++i)
    {
        seq_t s1 = buf[(i - 1) % 13];          /* a(i-1)        */
        seq_t t  = buf[(i - 6) % 13];          /* a(i-6)        */
        s1 += t;                               /* + a(i-6)      */
        s1 += t;                               /* + a(i-6) again */
        s1 += buf[(i -13) % 13];               /* + a(i-13)     */

        seq_t s2 = buf[(i - 7) % 13];          /* a(i-7)        */
        s2 += s2;                               /* + a(i-7) again */
        s2 += buf[(i -12) % 13];               /* + a(i-12)     */

        buf[i % 13] = s1 - s2;                 /* store a(i)    */
    }

    return buf[n % 13];
}
