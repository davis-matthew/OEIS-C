/*
 * A347413  a(n) = (product of first n semiprimes) mod (sum of first n
 *          semiprimes).
 * https://oeis.org/A347413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A112141(n) mod A062198(n) (product of first n semiprimes)
 *            mod (sum of first n semiprimes)
 * Range:     u8 n <= 13, u16 n <= 219, u32 n <= 1057, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime.h"

/* cached lists of semiprimes, their cumulative sums */
static uint64_t *semirlist   = NULL;   /* list of semiprimes */
static size_t    semircap    = 0;      /* capacity of semirlist   */
static idx_t     semicount   = 0;      /* number of semiprimes stored */

static seq_t *prefix        = NULL;   /* cumulative sums of semiprimes */
static size_t  prefixcap    = 0;       /* capacity of prefix array  */

/* ensure we have at least 'n' semiprimes in the caches */
static void ensure_semiprimes(idx_t n)
{
    if (semicount >= n)
        return;

    /* grow semirlist if needed */
    if (semircap < (size_t)n) {
        size_t newcap = semircap ? semircap * 2 : 64;
        if (newcap < (size_t)n) newcap = n;
        uint64_t *p = (uint64_t *)realloc(semirlist, newcap * sizeof(uint64_t));
        if (!p) abort();
        semirlist = p;
        semircap  = newcap;
    }

    /* grow prefix if needed */
    if (prefixcap < (size_t)n) {
        size_t newcap = prefixcap ? prefixcap * 2 : 64;
        if (newcap < (size_t)n) newcap = n;
        seq_t *p = (seq_t *)realloc(prefix, newcap * sizeof(seq_t));
        if (!p) abort();
        prefix = p;
        prefixcap = newcap;
    }

    /* generate semiprimes until we reach 'n' */
    static uint64_t last_checked = 1;    /* start after 1 */
    while (semicount < n) {
        ++last_checked;                     /* next integer to test */
        uint64_t p = _smallest_prime_factor(last_checked);
        if (p == 0)                         /* not composite */
            continue;
        uint64_t q = last_checked / p;
        if (!is_prime(q))                   /* not exactly two prime factors */
            continue;
        /* found a semiprime */
        semirlist[semicount] = last_checked;
        if (semicount == 0)
            prefix[0] = last_checked;
        else
            prefix[semicount] = prefix[semicount-1] + last_checked;
        ++semicount;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; 0 is undefined */
        return 0;

    ensure_semiprimes(n);

    seq_t sum = prefix[n-1];

    /* compute product modulo sum */
    seq_t prod = 1 % sum;     /* prod = 1 (mod sum) */
    for (idx_t i = 0; i < n; ++i) {
        seq_t term = (seq_t)(semirlist[i] % (uint64_t)sum);
        prod = (prod * term) % sum;
    }

    return prod;
}
