/*
 * A357458  First differences of A325033 = "Sum of sums of the multiset of
 *          prime indices of each prime index of n.".
 * https://oeis.org/A357458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A325033(n+1) - A325033(n)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325033(n) = sum_{p^e||n} e * S[idx(p)] ,
 * where idx(p) is the index of prime p in the ordered list of primes
 * and S[k] = sum_{q^f||k} f * idx(q)  (sum of prime indices of k).  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ----- configuration ---------------------------------------------------------- */
#define MAX_LIMIT   20000          /*  covers n up to about 20000, more than enough  */

/* ----- global tables ---------------------------------------------------------- */
static uint32_t *primes          = NULL;  /* array of primes up to MAX_LIMIT       */
static size_t   primes_cnt       = 0;
static uint16_t *prime_index     = NULL;  /* prime_index[p] = index of prime p (1‑based) */
static uint32_t *S              = NULL;   /* S[k] = sum of prime indices of k      */
static bool     initialized     = false;
static size_t   limit_used      = 0;

/* ----- helpers --------------------------------------------------------------- */
static void init(void)
{
    if (initialized) return;
    initialized = true;
    limit_used = MAX_LIMIT;

    /* allocate tables */
    primes      = (uint32_t *)malloc((limit_used + 1) * sizeof(uint32_t));
    prime_index = (uint16_t *)calloc((limit_used + 1), sizeof(uint16_t));
    if (!primes || !prime_index) return;   /* allocation failure */

    /* sieve of Eratosthenes up to limit_used */
    uint8_t *is_composite = (uint8_t *)calloc((limit_used + 1), sizeof(uint8_t));
    if (!is_composite) return;
    for (size_t i = 2; i <= limit_used; ++i) {
        if (!is_composite[i]) {
            prime_index[(uint32_t)i] = (uint16_t)(primes_cnt + 1);
            primes[primes_cnt++] = (uint32_t)i;
            if (i <= limit_used / i) {
                for (size_t j = i * i; j <= limit_used; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);

    /* allocate S array of size primes_cnt + 1 */
    S = (uint32_t *)malloc((primes_cnt + 1) * sizeof(uint32_t));
    if (!S) return;

    /* compute S[k] for k = 1..primes_cnt                                     */
    for (size_t k = 1; k <= primes_cnt; ++k) {
        size_t val = k;
        uint32_t sum = 0;
        for (size_t pi = 0; pi < primes_cnt && (size_t)primes[pi] * (size_t)primes[pi] <= val; ++pi) {
            uint32_t p = primes[pi];
            if ((val % p) == 0) {
                uint32_t e = 0;
                while ((val % p) == 0) {
                    val /= p;
                    e++;
                }
                sum += e * prime_index[p];
            }
        }
        if (val > 1) {   /* remaining prime factor */
            sum += prime_index[(uint32_t)val];
        }
        S[k] = sum;
    }
}

/* ----- core computation ------------------------------------------------------- */
static seq_t A325033(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* A325033(1) = 0 */
    idx_t val = n;
    seq_t total = 0;
    for (size_t pi = 0; pi < primes_cnt && (size_t)primes[pi] * (size_t)primes[pi] <= val; ++pi) {
        uint32_t p = primes[pi];
        if ((val % p) == 0) {
            uint32_t e = 0;
            while ((val % p) == 0) {
                val /= p;
                e++;
            }
            uint32_t idxp = prime_index[p];
            total += (seq_t)e * (seq_t)S[idxp];
        }
    }
    if (val > 1) {   /* remaining prime factor */
        uint32_t idxp = prime_index[(uint32_t)val];
        total += (seq_t)S[idxp];
    }
    return total;
}

/* ----- public sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    init();                                 /* ensure tables are ready      */
    return A325033(n + 1) - A325033(n);
}
