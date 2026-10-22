/*
 * A369464  Numbers for which there is no representation as a sum (p*q + p*r +
 *          q*r) with three odd primes p <= q <= r.
 * https://oeis.org/A369464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Non‑representable integers (offset 1). a(n) is the nth integer
 *            that cannot be written as p*q + p*r + q*r with three odd primes
 *            p ≤ q ≤ r
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes_upto, int_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/int_sqrt.h"          /* idx_t int_sqrt(idx_t n); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                          uint64_t **out, size_t *cnt); */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n); */

/* ----------  Prime table (dynamic, up to sqrt(max_checked)) --------------- */
static uint64_t *prime_list = NULL;   /* Stored primes (including 2)      */
static size_t   prime_cnt  = 0;
static idx_t    prime_limit = 0;      /* Largest value we have pre‑prime up */

static void
ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;
    build_primes_upto(limit, &new_list, &new_cnt);

    if (prime_list != NULL)
        free(prime_list);

    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_limit = limit;
}

/* ----------  Test if x is representable --------------------------------- */
static bool
is_representable(idx_t x)
{
    /* smallest representable value: 3*3+3*3+3*3 = 27 */
    if (x < 27)
        return false;

    /* we need primes up to sqrt(x) + 1 for the search */
    ensure_prime_limit(int_sqrt(x) + 1);

    idx_t p_max = int_sqrt(x / 3) + 1;      /* 3p^2 <= x  →  p <= sqrt(x/3) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p == 2)                 /* skip even prime */
            continue;
        if (p > (uint64_t)p_max)
            break;

        for (size_t j = i; j < prime_cnt; ++j) {
            uint64_t q = prime_list[j];
            if (q == 2)
                continue;
            if (q < p)
                continue;

            /* if p*q already exceeds x, no further q will work */
            if ((uint64_t)p * (uint64_t)q > (uint64_t)x)
                break;

            /* r = (x - p*q) / (p + q) must be integer prime >= q   */
            uint64_t r_num = (uint64_t)x - (uint64_t)p * (uint64_t)q;
            if (r_num == 0)
                continue;  /* would give r = 0 */
            uint64_t denom = (uint64_t)p + (uint64_t)q;
            if (r_num % denom != 0)
                continue;
            uint64_t r = r_num / denom;
            if (r < q)
                continue;
            if ((r & 1ULL) == 0)          /* r must be odd */
                continue;
            /* final primality test */
            if (is_prime_big((seq_t)r))
                return true;
        }
    }
    return false;
}

/* ----------  Non‑representable cache ----------------------------------- */
static seq_t  *nonrep_list = NULL;  /* dynamic array */
static size_t  nonrep_cnt  = 0;
static size_t  nonrep_cap  = 0;
static idx_t   last_checked = 0;    /* next integer to test            */

static void
add_nonrep(idx_t val)
{
    if (nonrep_cnt >= nonrep_cap) {
        size_t new_cap = nonrep_cap ? 2 * nonrep_cap : 8;
        seq_t *tmp = (seq_t *)realloc(nonrep_list,
                                     new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* out of memory */
        nonrep_list = tmp;
        nonrep_cap  = new_cap;
    }
    nonrep_list[nonrep_cnt++] = (seq_t)val;
}

/* ----------  Main sequence function ------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)           /* offset is 1; undefined behaviour for n=0 */
        return 0;

    /* generate non‑representable numbers until we have n of them */
    while (nonrep_cnt < (size_t)n) {
        if (!is_representable(last_checked))
            add_nonrep(last_checked);
        ++last_checked;
    }
    return nonrep_list[n - 1];
}
