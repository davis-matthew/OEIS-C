/*
 * A303232  Total area of the family of rectangles with dimensions q and |q-p|
 *          such that p and q are prime, n = p + q and p < q.
 * https://oeis.org/A303232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} (n-i) * (n-2*i) * * (1 if i is
 *            prime and n-i is prime, else 0)
 * Range:     u8 n <= 19, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* static sieve state ------------------------------------------------------- */
static bool *prime_flag = NULL;   /* 1 if index is prime, 0 otherwise   */
static idx_t  sieve_limit = 0;    /* maximum index that has been sieved */

/* ensure the sieve covers all indices up to n (inclusive) */
static void
ensure_sieve(idx_t n)
{
    if (n <= sieve_limit)
        return;

    /* allocate new flag array (C++ needs an explicit cast) */
    bool *new_flag = (bool*)malloc((n + 1) * sizeof(bool));
    if (!new_flag)  /* allocation failure – in practice this won't happen */
        return;

    /* initialise all to prime (true) */
    for (idx_t i = 0; i <= n; ++i)
        new_flag[i] = true;

    if (n >= 0)
        new_flag[0] = false;
    if (n >= 1)
        new_flag[1] = false;

    /* simple sieve of Eratosthenes up to n */
    for (idx_t p = 2; p * p <= n; ++p)
        if (new_flag[p])
            for (idx_t m = p * p; m <= n; m += p)
                new_flag[m] = false;

    /* replace old array */
    free(prime_flag);
    prime_flag = new_flag;
    sieve_limit = n;
}

/* main sequence function --------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;  /* values below the offset are irrelevant */

    ensure_sieve(n);

    seq_t sum = 0;
    idx_t limit = (n - 1) / 2;

    for (idx_t i = 1; i <= limit; ++i) {
        if (prime_flag[i] && prime_flag[n - i]) {
            seq_t term = (seq_t)(n - i) * (seq_t)(n - 2 * i);
            sum += term;
        }
    }
    return sum;
}
