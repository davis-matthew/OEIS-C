/*
 * A242753  Number of ordered ways to write n = k + m with 0 < k <= m such
 *          that the inverse of k mod prime(k) among 1, ..., prime(k) - 1 is
 *          prime and the inverse of m mod prime(m) among 1, ..., prime(m) - 1
 *          is also prime.
 * https://oeis.org/A242753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (k,m) with 0<k<=m and k+m=n *
 *            such that the modular inverse of k modulo prime(k) is a prime, *
 *            and the analogous statement holds for m. * The implementation
 *            caches the “good” property for integers up to the * largest n
 *            seen so far. Each call runs in O(n) time
 * Range:     u8 n <= 60, u16 n <= 7737, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, not used */

    /* ---------- cache for good[n] --------------------------------------- */
    static bool *good   = NULL;          /* good[i] true iff inverse of i is prime */
    static size_t  alloc = 0;            /* allocated size of array             */
    static size_t  maxn  = 0;            /* highest index computed so far        */

    if ((size_t)n > maxn) {
        size_t new_alloc = (size_t)n + 1;   /* need indices 0..n (index 0 unused) */
        bool *tmp;

        if (good == NULL) {
            tmp = (bool*)calloc(new_alloc, sizeof(bool));
            if (!tmp) return 0;              /* allocation failure */
        } else {
            tmp = (bool*)realloc(good, new_alloc * sizeof(bool));
            if (!tmp) return 0;              /* allocation failure */
            /* initialize newly allocated part to false (0) */
            memset(tmp + alloc, 0, (new_alloc - alloc) * sizeof(bool));
        }
        good  = tmp;
        alloc = new_alloc;

        /* compute good values for new indices */
        for (size_t i = maxn + 1; i <= (size_t)n; ++i) {
            uint32_t pk  = get_prime((idx_t)i);            /* i-th prime            */
            seq_t   inv = mod_pow((seq_t)i, (uint64_t)pk - 2,
                                   (seq_t)pk);              /* inverse of i mod pk   */
            good[i] = is_prime((idx_t)inv);                /* inverse prime?        */
        }
        maxn = new_alloc - 1;
    }

    /* ---------- evaluate a(n) ------------------------------------------ */
    seq_t count = 0;
    for (idx_t k = 1; k <= n / 2; ++k) {
        idx_t m = n - k;
        if (good[(size_t)k] && good[(size_t)m])
            ++count;
    }
    return count;
}
