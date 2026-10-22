/*
 * A348440  Records in A088177.
 * https://oeis.org/A348440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n ≥ 14, a(n) is the (n–4)-th prime number. The first 13
 *            terms are hard‑coded because they are record values of the
 *            so‑called A088177 sequence before it aligns with the primes
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Forward declaration of the function */
seq_t a(idx_t n);

/* ---------------------------------------------------------------------- */
/*   Helper to generate primes up to a given limit (inclusive)             */
/* ---------------------------------------------------------------------- */
static uint64_t *primes_arr = NULL;     /* array of primes  (0‑based)       */
static size_t   prime_count = 0;        /* number of primes stored          */
static size_t   sieve_limit = 0;        /* last limit used for the sieve     */

static void build_primes_up_to(size_t limit)
{
    bool *sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sieve) return;                      /* out of memory (unlikely)         */

    for (size_t i = 0; i <= limit; ++i)
        sieve[i] = true;
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }

    /* count primes */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++count;

    /* allocate or resize array */
    uint64_t *tmp = (uint64_t *)realloc(primes_arr, count * sizeof(uint64_t));
    primes_arr = tmp;
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) primes_arr[idx++] = (uint64_t)i;

    prime_count = count;
    sieve_limit = limit;
    free(sieve);
}

/* ---------------------------------------------------------------------- */
/*   Main sequence routine                                              */
/* ---------------------------------------------------------------------- */
static const seq_t early[13] = {
    (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)5,  (seq_t)6,  (seq_t)7,
    (seq_t)11, (seq_t)12, (seq_t)13, (seq_t)17, (seq_t)19, (seq_t)23,
    (seq_t)25
};

seq_t a(idx_t n)
{
    /* Handle the first 13 record values explicitly */
    if (n <= 13)
        return early[n - 1];

    /* For n ≥ 14, a(n) = (n–4)-th prime */
    idx_t kth = n - 4;                       /* 1‑based prime index */

    /* Ensure we have at least kth primes */
    if ((size_t)prime_count < (size_t)kth) {
        size_t needed = (size_t)kth;
        size_t limit;

        if (needed < 6)
            limit = 15;
        else {
            /* Rough upper bound: p_k < k (log k + log log k) for k ≥ 6  */
            double d = (double)needed;
            limit = (size_t)(d * (log(d) + log(log(d)))) + 10;
        }

        /* Rebuild primes until we have enough */
        while (prime_count < needed) {
            build_primes_up_to(limit);
            if (prime_count >= needed)
                break;
            /* If still not enough, double the limit and try again */
            limit *= 2;
        }
    }

    /* Return the (kth‑1)-th entry from 0‑based array */
    return (seq_t)primes_arr[kth - 1];
}
