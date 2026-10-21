/*
 * A276063  Sum of the asymmetry degrees of all compositions of n with parts
 *          in {1,4}.
 * https://oeis.org/A276063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k>=0} k*A276062(n,k). Computed via the linear
 *            recurrence obtained from the generating function
 *            g(z)=2*z^5/((1+z+z^4)*(1-z-z^4)^2): a(n) =
 *            a(n-1)+a(n-2)-a(n-3)+a(n-4)+2*a(n-5)-3*a(n-6)+a(n-8)-3*a(n-9)-a(n-12)
 *            with initial values a(0)=a(1)=a(2)=a(3)=a(4)=0 and a(5)=2. The
 *            sequence is de...
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 64, u64 n <= 131,
 *            u128 n <= 266, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* circular buffer containing the last 13 values a(n-12) … a(n-1). */
    seq_t buf[13] = {0};

    for (idx_t i = 0; i <= n; ++i) {
        /* Right–hand side value – only non‑zero for i==5. */
        seq_t rhs = (i == 5) ? (seq_t)2 : (seq_t)0;

        /* Compute the sum of the positive terms. */
        seq_t pos = rhs;
        if (i >= 1) pos += buf[(i - 1) % 13];
        if (i >= 2) pos += buf[(i - 2) % 13];
        if (i >= 4) pos += buf[(i - 4) % 13];
        if (i >= 5) pos += (seq_t)2 * buf[(i - 5) % 13];
        if (i >= 8) pos += buf[(i - 8) % 13];

        /* Compute the sum of the negative terms. */
        seq_t neg = (seq_t)0;
        if (i >= 3) neg += buf[(i - 3) % 13];
        if (i >= 6) neg += (seq_t)3 * buf[(i - 6) % 13];
        if (i >= 9) neg += (seq_t)3 * buf[(i - 9) % 13];
        if (i >= 12) neg += buf[(i - 12) % 13];

        /* The recurrence guarantees pos ≥ neg, so subtraction cannot underflow. */
        seq_t val = pos - neg;

        /* Store the new value and continue. */
        buf[i % 13] = val;
    }

    return buf[n % 13];
}
