/*
 * A291635  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers such that p = x + 2*y + 5*z, p - 2, p + 4 and
 *          p + 10 are all prime.
 * https://oeis.org/A291635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of non‑negative integer quadruples (x,y,z,w) such
 *            that x² + y² + z² + w² = n and with p = x + 2*y + 5*z, the
 *            numbers p‑2, p, p+4 and p+10 are all prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* lazy static sieve of primes up to 1000 (p+10 ≤ 810 when n ≤ 10000) */
    static bool prime[1001];
    static bool primes_computed = false;
    if (!primes_computed) {
        for (int i = 0; i <= 1000; ++i) prime[i] = true;
        prime[0] = prime[1] = false;
        for (int i = 2; i * i <= 1000; ++i) {
            if (prime[i]) {
                for (int j = i * i; j <= 1000; j += i)
                    prime[j] = false;
            }
        }
        primes_computed = true;
    }

    /* pre‑compute squares up to 100 (since n ≤ 10000 in the test range) */
    static int sq[101];
    static bool sq_computed = false;
    if (!sq_computed) {
        for (int i = 0; i <= 100; ++i) sq[i] = i * i;
        sq_computed = true;
    }

    seq_t count = 0;
    /* Square‑root of n for the outermost loop limit */
    int sqrt_n = (int)sqrt((double)n);
    for (int x = 0; x <= sqrt_n; ++x) {
        if ((idx_t)sq[x] > n) break;
        int rem_xy = (int)(n - (idx_t)sq[x]);

        int sqrt_y = (int)sqrt((double)rem_xy);
        for (int y = 0; y <= sqrt_y; ++y) {
            if (sq[x] + sq[y] > n) break;
            int rem_xyz = rem_xy - sq[y];

            int sqrt_z = (int)sqrt((double)rem_xyz);
            for (int z = 0; z <= sqrt_z; ++z) {
                if (sq[x] + sq[y] + sq[z] > n) break;
                int p = x + 2 * y + 5 * z;
                if (p < 2) continue;                 /* p must be ≥ 2 */
                /* check the required prime quadruple */
                if (!prime[p - 2] || !prime[p] || !prime[p + 4] || !prime[p + 10])
                    continue;

                int w2 = n - (idx_t)(sq[x] + sq[y] + sq[z]);
                int w = (int)(sqrt((double)w2) + 0.5);  /* nearest integer */
                if ((idx_t)w * (idx_t)w != w2) continue;/* w must be exact */

                /* all conditions satisfied */
                ++count;
            }
        }
    }
    return (seq_t)count;
}
