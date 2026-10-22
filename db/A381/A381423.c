/*
 * A381423  Exponent of x of maximal coefficient in Hermite polynomial of
 *          order n.
 * https://oeis.org/A381423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exponent of x of maximal coefficient in Hermite polynomial H_n(x).  */
/* Compute a(n) by iteratively comparing consecutive even k-values. */
seq_t a(idx_t n)
{
    /* floor(n/2) – the largest even k corresponds to k = floor(n/2)   */
    idx_t half = n >> 1;          /* floor(n/2)                */
    idx_t k    = 0;               /* current even k            */
    idx_t m    = n;               /* current exponent (n-2k)   */

    for (;;) {
        idx_t next_k = k + 2;
        if (next_k > half) break; /* no further even k            */

        /* Evaluate the ratio R = V_{k+2} / V_k.  We need only to know
         * if R > 1.  From the analytic form of V_k we have:
         *
         *   R = 1/16 * (m * (m-1) * (m-2) * (m-3)) / ((k+1)*(k+2))
         *
         *  So R>1  <=>  m*(m-1)*(m-2)*(m-3) > 16*(k+1)*(k+2).
         */
        unsigned long long mm   = (unsigned long long)m;
        unsigned long long num  = mm * (mm-1) * (mm-2) * (mm-3);
        unsigned long long den  = ((unsigned long long)(k+1))
                               *  ((unsigned long long)(k+2));
        unsigned long long bound = 16ULL * den;

        if (num > bound) {
            /* The coefficient increases when k increases by 2. */
            k  = next_k;
            m  = n - 2 * next_k;
            continue;
        }
        break;            /* the previous k gives the maximum coefficient */
    }

    return (seq_t)m;
}
