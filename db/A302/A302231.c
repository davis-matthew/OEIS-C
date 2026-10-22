/*
 * A302231  Number of pairs of Goldbach partitions of 2n, (p,q) and (s,t) with
 *          p < s <= t < q such that s = p + 2 and t = q - 2.
 * https://oeis.org/A302231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence counts, for each n ≥ 1, the number of pairs of *
 *            Goldbach partitions of 2n that are spaced by twice the
 *            twin‑prime * distance. Equivalently it is the number of primes p
 *            with p, p+2, * q = 2n – p and q–2 all prime and p < q, with p ≤
 *            q–4. * * Implementation: we pre‑compute a sieve of...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*--- static prime cache ----------------------------------------*/
static bool *prime_cache = NULL;   /* array of bools: prime_cache[i] == true <=> i is prime */
static idx_t prime_cache_size = 0; /* number of entries in prime_cache (== limit+1) */

/* Ensure prime_cache[0..limit] is filled (≤ 2*n maximum). */
static void init_prime_sieve(idx_t limit)
{
    if (prime_cache_size > limit) return;          /* already large enough */

    /* recycle old array */
    free(prime_cache);
    prime_cache = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!prime_cache) exit(1);                     /* out of memory: abort */

    /* initialize all entries to true */
    for (idx_t i = 0; i <= limit; ++i) prime_cache[i] = true;
    if (limit >= 0) prime_cache[0] = false;
    if (limit >= 1) prime_cache[1] = false;

    /* sieve */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (prime_cache[i])
            for (idx_t j = i * i; j <= limit; j += i)
                prime_cache[j] = false;

    prime_cache_size = limit + 1;
}

/*--- main sequence function ------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* no defined values below the offset */

    /* Build sieve up to 2*n (primes needed up to 2*n). */
    init_prime_sieve(2 * n);

    seq_t cnt = 0;

    /* iterate over possible lower partner primes p */
    for (idx_t p = 3; p <= n - 1; ++p) {
        if (!prime_cache[p]) continue;               /* p not prime */
        if (p + 2 >= prime_cache_size || !prime_cache[p + 2]) continue; /* p+2 not prime */

        idx_t q = 2 * n - p;
        if (q <= p) continue;                       /* need p < q */
        if (q >= prime_cache_size || !prime_cache[q]) continue;     /* q not prime */
        if (q < 2 || q - 2 >= prime_cache_size || !prime_cache[q - 2]) continue; /* q-2 not prime */

        /* ensure s ≤ t: i.e., p + 2 ≤ q – 2 ⇔ p ≤ q – 4 */
        if (p > q - 4) continue;

        ++cnt;
    }

    return cnt;
}
