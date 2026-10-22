/*
 * A368610  a(n) = Sum_{k=1..n} pi(k) * ceiling(n/k).
 * https://oeis.org/A368610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} pi(k) * ceil(n/k)
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ---------- internal prime sieve & pi cache ---------- */
static uint8_t *prime_flag = NULL;     /* 1 => prime, 0 => composite */
static size_t  sieve_size = 0;         /* largest index that prime_flag covers */

static seq_t *pi_cache = NULL;         /* pi_cache[i] = pi(i) for 0 <= i <= pi_size */
static size_t pi_size   = 0;

/* extend prime_flag array to cover [0..new_n] */
static void extend_sieve(idx_t new_n)
{
    if ((idx_t)new_n <= (idx_t)sieve_size)
        return;

    uint8_t *new_flag = (uint8_t *)calloc((size_t)new_n + 1, 1);
    if (!new_flag)
        return;          /* allocation failure – unlikely for the intended n */

    /* initialise as "prime" */
    for (idx_t i = 2; i <= new_n; ++i)
        new_flag[i] = 1;

    /* sieve */
    size_t limit = (size_t)sqrt((double)new_n);
    for (size_t i = 2; i <= limit; ++i) {
        if (new_flag[i]) {
            for (size_t j = i * i; j <= (size_t)new_n; j += i)
                new_flag[j] = 0;
        }
    }
    new_flag[0] = new_flag[1] = 0;

    /* replace old flag array */
    free(prime_flag);
    prime_flag = new_flag;
    sieve_size = new_n;
}

/* extend pi_cache to include all values up to new_n */
static void extend_pi(idx_t new_n)
{
    if ((idx_t)new_n <= (idx_t)pi_size)
        return;

    /* ensure prime_flag covers up to new_n */
    extend_sieve(new_n);

    size_t new_size = (size_t)new_n + 1;
    seq_t *new_pi = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_pi)
        return;          /* allocation failure – unlikely for the intended n */

    new_pi[0] = 0;
    for (idx_t i = 1; i <= new_n; ++i)
        new_pi[i] = new_pi[i - 1] + (seq_t)prime_flag[i];

    free(pi_cache);
    pi_cache = new_pi;
    pi_size  = new_n;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* out of OEIS range */

    extend_pi(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pk   = pi_cache[k];
        seq_t ceil = ((seq_t)n + k - 1) / k;   /* ceil(n/k) */
        sum += pk * ceil;
    }
    return sum;
}
