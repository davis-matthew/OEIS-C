/*
 * A333534  a(n) is the number of log(n)-smooth numbers <= n.
 * https://oeis.org/A333534
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k ≤ n whose greatest prime factor is ≤ ⌊log(n)⌋
 * Range:     u8 n <= 4499, u16 n <= 8349, u32 n <= 8349, u64 n <= 8349,
 *            u128 n <= 8349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

/* Count numbers ≤ n with all prime factors ≤ ⌊log(n)⌋.
   idx_t is an unsigned integer type (e.g., uint64_t).
   seq_t is the result type (e.g., uint32_t, uint64_t, unsigned __int128). */
seq_t a(idx_t n)
{
    if (n < 2) {                 /* sequence starts at n = 2 */
        return 0;
    }

    /* L = ⌊log(n)⌋  (natural logarithm). */
    size_t L = (size_t)floor(log((double)n));

    /* Build smallest‑prime‑factor array up to n. */
    size_t N = (size_t)n;
    unsigned int *spf = (unsigned int*)malloc((N + 1) * sizeof(unsigned int));
    if (!spf) {                   /* allocation failed: return 0 as fallback */
        return 0;
    }
    for (size_t i = 0; i <= N; ++i) {
        spf[i] = 0;
    }
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = (unsigned int)i;
            if (i * i <= N) {
                for (size_t j = i * i; j <= N; j += i) {
                    if (spf[j] == 0) {
                        spf[j] = (unsigned int)i;
                    }
                }
            }
        }
    }

    /* Count qualifying numbers. */
    seq_t count = 0;
    for (size_t k = 1; k <= N; ++k) {
        unsigned int maxp = 1;
        size_t x = k;
        while (x > 1) {
            unsigned int p = spf[x];
            maxp = p;                 /* the largest prime factor seen so far */
            while (x % p == 0) {
                x /= p;
            }
        }
        if (maxp <= (unsigned int)L) {
            ++count;
        }
    }

    free(spf);
    return count;
}
