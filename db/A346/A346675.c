/*
 * A346675  First differences of A088176.
 * https://oeis.org/A346675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A088176(n+1) – A088176(n) A088176(k) is the k‑th prime p
 *            such that the previous prime q is the larger member of a twin
 *            prime pair (q–2 is also prime)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t       current_limit = 0;  /* upper limit used by the sieve */

static seq_t      *qual          = NULL;  /* qualifying primes (A088176) */
static size_t       qual_cnt      = 0;     /* number of qualifiers stored */
static size_t       qual_cap      = 0;     /* allocated capacity of qual[] */

/* Ensure that at least 'needed' qualifiers are available. */
static void ensure_qualifiers(idx_t needed)
{
    if (needed <= qual_cnt)
        return;
    if (current_limit == 0)
        current_limit = 1000;          /* start with a modest limit */

    while (qual_cnt < needed) {
        /* Double the limit each time we need more qualifiers. */
        current_limit <<= 1;

        /* Allocate primality table. 0 => composite, 1 => prime. */
        char *is_prime = (char *)calloc((size_t)current_limit + 1, 1);
        if (!is_prime) exit(1);          /* out of memory */

        /* Initially assume all numbers >=2 are prime. */
        for (idx_t i = 2; i <= current_limit; ++i)
            is_prime[i] = 1;

        /* Mark composites. */
        for (idx_t i = 2; i * i <= current_limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= current_limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Build the list of qualifying primes. */
        qual_cnt = 0;
        if (qual_cap < 16) {
            qual_cap = 16;
            qual = (seq_t *)realloc(qual, qual_cap * sizeof(seq_t));
        }

        idx_t last_prime = 0;
        for (idx_t i = 2; i <= current_limit; ++i) {
            if (is_prime[i]) {
                if (last_prime >= 3 && is_prime[last_prime - 2]) {
                    if (qual_cnt == qual_cap) {
                        qual_cap <<= 1;              /* grow capacity */
                        qual = (seq_t *)realloc(qual, qual_cap * sizeof(seq_t));
                    }
                    qual[qual_cnt++] = (seq_t)i;     /* store as seq_t */
                }
                last_prime = i;
            }
        }
        free(is_prime);   /* primality table no longer needed */
    }
}

/* Return the n-th term of sequence A346675. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n < 1 */

    /* Need at least n+1 qualifiers to compute the difference. */
    ensure_qualifiers(n + 1);

    return qual[n] - qual[n - 1];
}
