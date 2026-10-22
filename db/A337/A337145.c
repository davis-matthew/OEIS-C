/*
 * A337145  a(n) is the determinant of the 2 X 2 matrix whose entries (when
 *          read by rows) are the n-th primes congruent to 1, 3, 5, 7 mod 8
 *          respectively.
 * https://oeis.org/A337145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = p1(n)*p7(n) - p3(n)*p5(n) * where p_r(n) is the n‑th
 *            prime ≡ r (mod 8) (r = 1,3,5,7)
 * Range:     i8 n <= 1, i16 n <= 16, i32 n <= 9210, i64 n <= 9886,
 *            i128 n <= 9886, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/sieve_primes.h"   /* provides static sieve_primes */

static uint64_t *primes = NULL;         /* cached primes */
static size_t    primes_cnt = 0;        /* number of cached primes  */
static idx_t     max_n_needed = 0;      /* largest n for which primes are cached */

/* ------------------------------------------------------------------ */
/*  Ensure that at least n primes of each residue 1,3,5,7 are cached. */
/* ------------------------------------------------------------------ */
static void
ensure_primes_for(idx_t n)
{
    if (n <= max_n_needed)
        return;               /* we already have enough primes */

    uint64_t limit = 1000;       /* initial sieve limit */
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;

    while (1) {
        /* free previous temporary array if any */
        if (new_primes != NULL) {
            free(new_primes);
            new_primes = NULL;
        }

        /* sieve up to current limit */
        sieve_primes(limit, &new_primes, &new_cnt);

        /* count residues */
        int cnt[8] = {0};
        for (size_t i = 0; i < new_cnt; ++i) {
            int r = (int)(new_primes[i] & 7);
            if (r < 8)
                cnt[r] += 1;
        }

        if (cnt[1] >= n && cnt[3] >= n && cnt[5] >= n && cnt[7] >= n) {
            /* sufficient primes – replace cache */
            free(primes);
            primes      = new_primes;
            primes_cnt  = new_cnt;
            max_n_needed = n;
            break;
        }

        /* not enough – double limit and try again */
        free(new_primes);
        new_primes = NULL;
        limit <<= 1;            /* multiply by 2 */
    }
}

/* ------------------------------------------------------------------ */
/*  Get the n‑th prime ≡ res (mod 8).                                 */
/* ------------------------------------------------------------------ */
static seq_t
nth_prime_mod8(idx_t n, int res)
{
    uint64_t count = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        if ((primes[i] & 7) == res) {
            ++count;
            if (count == n)
                return (seq_t)primes[i];
        }
    }
    /* Should never reach here if ensure_primes_for has been called. */
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)                /* offset is 1; a(0) undefined */
        return 0;

    ensure_primes_for(n);

    seq_t p1 = nth_prime_mod8(n, 1);
    seq_t p3 = nth_prime_mod8(n, 3);
    seq_t p5 = nth_prime_mod8(n, 5);
    seq_t p7 = nth_prime_mod8(n, 7);

    return p1 * p7 - p3 * p5;
}
