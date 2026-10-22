/*
 * A392263  Numbers k such that sigma(k) = psi(k) + tau(k) + omega(k)^3.
 * https://oeis.org/A392263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392263
   a(n) = 13 * p^2   where p is the nth prime, except the prime 13 is
   skipped.  Thus a(1)=13*2^2=52, a(2)=13*3^2=117, ...  */
/* Implementation is based on an incremental prime generator. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static idx_t *primes = NULL;          /* array of discovered primes   */
static size_t primes_len = 0;         /* number of primes stored      */
static size_t primes_cap = 0;         /* allocated capacity of `primes` */

/* Ensure that primes[0 .. target_idx-1] are filled (1‑indexed). */
static void
ensure_primes_until(idx_t target_idx)
{
    while (primes_len < target_idx) {
        /* Find the next prime after the last stored one. */
        idx_t candidate = (primes_len == 0) ? 2 : primes[primes_len - 1] + 1;
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                idx_t p = primes[i];
                /* stop if p > sqrt(candidate) without overflow */
                if (p > candidate / p) break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                /* add this prime to the array */
                if (primes_cap <= primes_len) {
                    primes_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
                    primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                }
                primes[primes_len++] = candidate;
                break;     /* prime found; exit inner loop */
            }
            ++candidate;
        }
    }
}

/* Return the nth term of A392263 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 0 */

    /* The prime 13 is omitted; it is the 6th prime. */
    idx_t prime_index = (n < 6) ? n : n + 1;

    ensure_primes_until(prime_index);

    idx_t p = primes[prime_index - 1];
    seq_t result = (seq_t)p;
    result = result * result;   /* p^2  */
    result = result * (seq_t)13; /* 13 * p^2 */
    return result;
}
