/*
 * A309716  Sum of the even parts appearing among the third largest parts of
 *          the partitions of n into 4 parts.
 * https://oeis.org/A309716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n^4+28*n^3+200*n+4096)/13824 + (n mod
 *            2)*(n^2+2*n)*7/2304 - ((n+2) mod 3)*n/54 + (((n+3) mod 4)+((n+2)
 *            mod 4)-((n+1) mod 4)-(n mod 4))*n/256 + ((n mod 6)+((n+1) mod
 *            6)-((n+3) mod 6)-((n+4) mod 6))*n/108 + (((n+5) mod 8)+((n+4)
 *            mod 8)-((n+1) mod 8)-(n mod 8))*n/128 ); The sum is ev...
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 64‑bit signed arithmetic for the intermediate sum.
       For n up to about 10 000 (the practical test range) this
       is safe and keeps the function O(1).  */
    unsigned long long x = (unsigned long long)n;   /* cast for modulo ops */

    /* term 1: 7*n^4 + 28*n^3 + 200*n + 4096 (over 13824) */
    long long term1 = 7ULL * x * x * x * x
                     + 28ULL * x * x * x
                     + 200ULL * x
                     + 4096ULL;

    /* term 2: (n mod 2)*(n^2+2*n)*7*6 /13824  (multiplied by 6 → /2304) */
    long long term2 = ((n & 1ULL) ? 1 : 0)
                     * ((long long)(x * x + 2ULL * x))
                     * 42LL;   /* 7*6 */

    /* term 3: -((n+2) mod 3)*n*256 /13824   (multiplied by 256 → /54) */
    long long term3 = - (long long)((n + 2ULL) % 3ULL)
                      * (long long)x
                      * 256LL;

    /* term 4: (((n+3) mod 4)+((n+2) mod 4)-((n+1) mod 4)-(n mod 4))*n*54 /13824 */
    long long m4 = (long long)((n + 3ULL) % 4ULL)
                 + (long long)((n + 2ULL) % 4ULL)
                 - (long long)((n + 1ULL) % 4ULL)
                 - (long long)(n % 4ULL);
    long long term4 = m4 * (long long)x * 54LL;   /* 54 = 13824/256 */

    /* term 5: ((n mod 6)+((n+1) mod 6)-((n+3) mod 6)-((n+4) mod 6))*n*128 /13824 */
    long long m5 = (long long)(n % 6ULL)
                 + (long long)((n + 1ULL) % 6ULL)
                 - (long long)((n + 3ULL) % 6ULL)
                 - (long long)((n + 4ULL) % 6ULL);
    long long term5 = m5 * (long long)x * 128LL;  /* 128 = 13824/108 */

    /* term 6: (((n+5) mod 8)+((n+4) mod 8)-((n+1) mod 8)-(n mod 8))*n*108 /13824 */
    long long m6 = (long long)((n + 5ULL) % 8ULL)
                 + (long long)((n + 4ULL) % 8ULL)
                 - (long long)((n + 1ULL) % 8ULL)
                 - (long long)(n % 8ULL);
    long long term6 = m6 * (long long)x * 108LL;  /* 108 = 13824/128 */

    long long total = term1 + term2 + term3 + term4 + term5 + term6;

    seq_t result = (seq_t)(total / 13824LL);
    return result;
}
