/*
 * A275246  Sequence of pairwise relatively prime numbers of class P_3 (see
 *          comment).
 * https://oeis.org/A275246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2*n-2)*prime(2*n-1) for n>1; a(1)=8
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_limit = 0;

/* Ensure that at least `need` primes are generated and stored in `primes`. */
static void ensure_prime_count(size_t need)
{
    if (prime_cnt >= need)
        return;

    size_t limit = prime_limit > 0 ? prime_limit : 1000;
    for (;;) {
        uint64_t *tmp = NULL;
        size_t cnt = 0;
        build_primes_upto((idx_t)limit, &tmp, &cnt);
        if (cnt >= need) {
            free(primes);
            primes      = tmp;
            prime_cnt   = cnt;
            prime_limit = limit;
            break;
        }
        free(tmp);
        /* Double the limit and try again. */
        if (limit < 16)
            limit = 16;
        else
            limit <<= 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)8;

    idx_t p1 = 2 * n - 2;      /* 1‑based index of the first prime */
    idx_t p2 = 2 * n - 1;      /* 1‑based index of the second prime */

    if ((size_t)p2 > prime_cnt)
        ensure_prime_count((size_t)p2);

    /* Array indices are 0‑based. */
    seq_t v1 = (seq_t)primes[(size_t)p1 - 1];
    seq_t v2 = (seq_t)primes[(size_t)p2 - 1];
    return v1 * v2;
}
