/*
 * A254681  Fifth partial sums of fourth powers (A000583).
 * https://oeis.org/A254681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(1 + n)*(2 + n)*(3 + n)*(4 + n)*(5 + n)^2*(5 +
 *            2*n)/30240
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 31, u64 n <= 400, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t prod = 1;

    /* Numerator factors */
    seq_t terms[8];
    terms[0] = N * N;          /* n^2          */
    terms[1] = N + 1;          /* 1 + n        */
    terms[2] = N + 2;          /* 2 + n        */
    terms[3] = N + 3;          /* 3 + n        */
    terms[4] = N + 4;          /* 4 + n        */
    terms[5] = N + 5;          /* 5 + n        */
    terms[6] = N + 5;          /* 5 + n (2nd)  */
    terms[7] = 5 + 2 * N;      /* 5 + 2*n      */

    /* Denominator prime factors 30240 = 2^5 * 3^3 * 5 * 7 */
    const int primes[4] = {2, 3, 5, 7};
    int   cnt[4]      = {5, 3, 1, 1};

    for (int i = 0; i < 8; ++i) {
        seq_t x = terms[i];
        for (int j = 0; j < 4; ++j) {
            while (cnt[j] > 0 && (x % primes[j]) == 0) {
                x /= (seq_t)primes[j];
                --cnt[j];
            }
        }
        prod *= x;
    }

    return prod;
}
