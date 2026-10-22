/*
 * A308256  Sum of the largest side lengths of all integer-sided triangles
 *          with perimeter n and nonprime sides.
 * https://oeis.org/A308256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of largest side lengths of all integer–sided *
 *            triangles with perimeter n and nonprime sides. * (Computation by
 *            exhaustive search over possible largest side lengths.)
 * Range:     u8 n <= 43, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Prime sieve cached across calls.
       prime[i] == true  -> i is prime
       prime[i] == false -> i is composite (or <2) */
    static bool *prime = NULL;
    static idx_t prime_max = 0;

    /* Expand the prime sieve if needed. */
    if (n > prime_max) {
        idx_t new_max = n;
        bool *p = (bool *)malloc((new_max + 1) * sizeof(bool));
        if (!p) return (seq_t)0;        /* allocation failed; return 0 */

        for (idx_t i = 0; i <= new_max; ++i) p[i] = true;
        p[0] = p[1] = false;

        for (idx_t i = 2; i * i <= new_max; ++i) {
            if (p[i]) {
                for (idx_t j = i * i; j <= new_max; j += i)
                    p[j] = false;
            }
        }

        free(prime);
        prime = p;
        prime_max = new_max;
    }

    seq_t sum = (seq_t)0;

    /* The largest side c satisfies ceil(n/3) <= c < n/2. */
    idx_t c_start = (n % 3 == 0) ? (n / 3) : (n / 3 + 1);
    idx_t c_end   = (n - 1) / 2;            /* floor((n-1)/2) */

    for (idx_t c = c_start; c <= c_end; ++c) {
        if (prime[c]) continue;              /* c must be nonprime */

        idx_t S = n - c;                     /* a + b = S */

        /* Minimum a so that b <= c: a >= n - 2c */
        idx_t a_low = (n > 2 * c) ? (n - 2 * c) : 1;
        /* Upper bound from a <= b: a <= S/2 */
        idx_t a_high = S / 2;

        if (a_low > a_high) continue;

        for (idx_t a = a_low; a <= a_high; ++a) {
            if (prime[a]) continue;          /* a nonprime required */
            idx_t b = S - a;
            if (prime[b]) continue;          /* b nonprime required */

            /* a <= b <= c holds by construction. */
            sum += (seq_t)c;
        }
    }

    return sum;
}
