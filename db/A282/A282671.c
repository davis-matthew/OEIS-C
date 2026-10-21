/*
 * A282671  Twice composite numbers.
 * https://oeis.org/A282671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2 * A002808(n) A002808(n) = n-th composite number (4, 6,
 *            8, 9, 10, 12, …) The implementation builds a sieve of primes up
 *            to a limit that is repeatedly doubled until enough composites
 *            have been found for the requested n. The sieve and the list of
 *            composites are cached in static storage so t...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of integer types provided by the build.  */
/* typedef ... idx_t;  */
/* typedef ... seq_t; */

static char   *sieve  = NULL;     /* 0=prime, 1=composite flag */
static seq_t  *comps  = NULL;     /* list of composite numbers */
static size_t  comps_len = 0;      /* number of composites stored */
static idx_t   sieve_limit = 0;    /* highest number sieved for */

/* Ensure that comps contains at least the first n composites.  */
static void
ensure_composites(idx_t n)
{
    if (comps_len >= (size_t)n)
        return;                       /* already have enough */

    idx_t needed_limit = n * (idx_t)3 + (idx_t)100;
    if (needed_limit < (idx_t)200)
        needed_limit = (idx_t)200;

    /* Rebuild the sieve and composite list until we have enough.  */
    while (comps_len < (size_t)n) {
        idx_t new_limit = sieve_limit ? sieve_limit * 2 : needed_limit;
        if (new_limit < needed_limit)
            new_limit = needed_limit;

        /* allocate new sieve */
        free(sieve);
        sieve = (char *)malloc((new_limit + 1) * sizeof(char));
        if (!sieve) {
            /* memory allocation failure – abort with zero; not expected */
            return;
        }
        for (idx_t i = 0; i <= new_limit; ++i)
            sieve[i] = 1;              /* assume composite initially */
        sieve[0] = sieve[1] = 0;       /* 0 and 1 are not prime */

        for (idx_t p = 2; p * p <= new_limit; ++p) {
            if (sieve[p]) {
                for (idx_t j = p * p; j <= new_limit; j += p)
                    sieve[j] = 0;       /* mark as prime */
            }
        }

        /* rebuild composite list */
        free(comps);
        comps = (seq_t *)malloc((new_limit + 1) * sizeof(seq_t));
        if (!comps) {
            /* memory allocation failure – abort with zero; not expected */
            return;
        }
        comps_len = 0;
        for (idx_t i = 4; i <= new_limit; ++i) {
            if (!sieve[i]) {
                comps[comps_len++] = (seq_t)i;
            }
        }
        sieve_limit = new_limit;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)               /* 0 is out of bounds for this offset  */
        return (seq_t)0;

    ensure_composites(n);

    /* 2 * the n-th composite (0‑based index n‑1)  */
    return (seq_t)comps[n - 1] * ((seq_t)2);
}
