/*
 * A375286  a(n) = f(1) + f(2) + ... + f(n), where f(n) = (-2)^Omega(n) =
 *          A165872(n).
 * https://oeis.org/A375286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Σ_{k=1..n} (-2)^{Ω(k)}
 * Range:     i8 n <= 127, i16 n <= 9993, i32 n <= 9993, i64 n <= 9993,
 *            i128 n <= 9993, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;              /* below offset */

    /* 1. Smallest prime factor sieve up to n */
    size_t N = (size_t)n;
    uint64_t *spf = (uint64_t*)malloc((N + 1) * sizeof(uint64_t));
    if (!spf)
        return (seq_t)0;              /* allocation failure */

    for (size_t i = 0; i <= N; ++i) spf[i] = 0;
    if (N >= 1) spf[1] = 1;

    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if (i <= N / i) {
                for (size_t j = i * i; j <= N; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    /* 2. Compute the sum */
    seq_t sum = 0;
    for (size_t k = 1; k <= N; ++k) {
        uint64_t temp = k;
        int omega = 0;
        while (temp > 1) {
            uint64_t p = spf[temp];
            if (p == 0) p = temp;        /* in case of a prime > sqrt(N) */
            temp /= p;
            ++omega;
        }

        seq_t val = 1;
        for (int i = 0; i < omega; ++i)
            val *= (seq_t)(-2);          /* multiply by -2 each time */

        sum += val;
    }

    free(spf);
    return sum;
}
