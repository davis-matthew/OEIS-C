/*
 * A279412  Expansion of Sum_{k>=1} prime(k)*x^prime(k)/(1 + x^prime(k)) *
 *          Product_{k>=1} (1 + x^prime(k)).
 * https://oeis.org/A279412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A000586(n), where A000586(n) is the number of
 *            partitions of n into distinct primes
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The build provides the integer types seq_t and idx_t.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* DP array indexed by n (size_t is fine because n is small).  */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return 0;
    for (size_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;                 /* empty partition */

    /* Simple sieve to enumerate primes up to n.  */
    size_t limit = N;
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!is_comp) { free(dp); return 0; }

    for (size_t p = 2; p <= limit; ++p) {
        if (is_comp[p]) continue;     /* composite, skip */
        /* Mark multiples of p as composite.  */
        for (size_t mult = p; mult <= limit; mult += p)
            is_comp[mult] = 1;

        /* Update DP using this prime.  */
        idx_t prime = (idx_t)p;      /* update loop uses unsigned idx_t */
        idx_t i2 = n;
        while (i2 >= prime) {
            dp[(size_t)i2] += dp[(size_t)(i2 - prime)];
            if (i2 == prime) break;   /* avoid underflow of unsigned */
            --i2;
        }
    }

    seq_t result = dp[N] * (seq_t)n;

    free(dp);
    free(is_comp);
    return result;
}
