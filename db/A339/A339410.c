/*
 * A339410  If the n-th semiprime is p*q with p<=q primes, a(n) is the area of
 *          the triangle with vertices (1,p), (p,q) and (q,p*q).
 * https://oeis.org/A339410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (q-1)*|p^2-q|/2 where p = smallest prime factor of the
 *            n-th semiprime, q = largest prime factor of the n-th semiprime
 * Range:     u8 n <= 20, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration */
static void init_once(void);

/* Global arrays holding the prime factors of semiprimes */
static seq_t *p_arr = NULL;
static seq_t *q_arr = NULL;
static idx_t   semiprime_cnt = 0;
static bool initialized = false;

static void
init_once(void)
{
    /* Upper limit chosen to contain at least 10000 semiprimes
       (≈ 50 000 is enough, we use 200 000 for safety). */
    const idx_t LIMIT = 200000;

    /* SPF array (smallest prime factor) */
    uint32_t *spf = (uint32_t *)calloc((size_t)(LIMIT + 1), sizeof(uint32_t));
    if (!spf) return;                       /* allocation failure → give up */

    /* Linear sieve for SPF */
    /* Allocate enough space for primes – π(LIMIT) < LIMIT/ln(LIMIT) */
    size_t  max_primes = LIMIT / 10;
    uint32_t *primes = (uint32_t *)malloc(max_primes * sizeof(uint32_t));
    if (!primes) { free(spf); return; }
    size_t   prime_cnt = 0;

    for (idx_t i = 2; i <= LIMIT; ++i)
    {
        if (spf[(size_t)i] == 0) {
            spf[(size_t)i] = (uint32_t)i;
            if (prime_cnt < max_primes) primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j)
        {
            idx_t p = primes[j];
            idx_t prod = i * p;
            if (prod > LIMIT) break;
            if (spf[(size_t)prod] == 0) spf[(size_t)prod] = (uint32_t)p;
            if (p == spf[(size_t)i]) break;
        }
    }

    /* First pass: count semiprimes up to LIMIT */
    idx_t count = 0;
    for (idx_t i = 4; i <= LIMIT; ++i)
    {
        uint32_t p = spf[(size_t)i];
        uint32_t q = (uint32_t)(i / p);
        if (spf[(size_t)q] == q) ++count;
    }

    /* Allocate arrays for the prime factors */
    p_arr = (seq_t *)malloc(count * sizeof(seq_t));
    q_arr = (seq_t *)malloc(count * sizeof(seq_t));
    if (!p_arr || !q_arr) {
        free(spf); free(primes);
        return;
    }

    /* Second pass: store factors */
    idx_t idx = 0;
    for (idx_t i = 4; i <= LIMIT; ++i)
    {
        uint32_t p = spf[(size_t)i];
        uint32_t q = (uint32_t)(i / p);
        if (spf[(size_t)q] == q)
        {
            p_arr[idx] = (seq_t)p;
            q_arr[idx] = (seq_t)q;
            ++idx;
        }
    }

    semiprime_cnt = idx;
    initialized   = true;

    free(spf);
    free(primes);
}

seq_t a(idx_t n)
{
    /* Ensure that the data structures are built */
    if (!initialized) init_once();

    if (n == 0 || n > semiprime_cnt) return (seq_t)0;

    seq_t p = p_arr[n - 1];
    seq_t q = q_arr[n - 1];
    seq_t p2 = p * p;

    seq_t diff = (p2 >= q) ? (p2 - q) : (q - p2);
    seq_t area = (q - 1) * diff / 2;

    return area;
}
